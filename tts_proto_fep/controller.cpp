#include "controller.h"

#include <coeaui.h>
#include <coecntrl.h>
#include <coemain.h>
#include <eikenv.h>

#include "app_reader.h"
#include "app_readers/contacts.h"
#include "app_readers/menu.h"
#include "app_readers/phone.h"
#include "control_walker.h"
#include "logging_window_gc.h"
#include "reporting.h"

static const TBool kDevelopmentMode = EFalse;

// TODO(mikie): move the Triggerer classes to their own file.
class ForegroundWalkTriggerer : public CBase, public MCoeForegroundObserver {
 public:
  ForegroundWalkTriggerer(TriggerInterface* walk, LoggingState* logger) {
    walk_ = walk;
    logger_ = logger;
    CEikonEnv::Static()->AddForegroundObserverL(*this);
  }
  virtual ~ForegroundWalkTriggerer() {
    CEikonEnv::Static()->RemoveForegroundObserver(*this);
  }
 private:
  virtual void HandleGainingForeground() {
    walk_->OnTrigger();
  }
  virtual void HandleLosingForeground() {}
  TriggerInterface* walk_;
  LoggingState* logger_;
};

class KeyPressWalkTriggerer : public CCoeControl {
 public:
  KeyPressWalkTriggerer(TriggerInterface* walk, LoggingState* logger) {
    walk_ = walk;
    logger_ = logger;
  }
  void ConstructL() {
    // Set up a key catching control - front window, null size, non-focusing,
    // added to stack for key events.
    CreateWindowL();
    SetNonFocusing();

    RWindow& window = Window();
    window.SetOrdinalPosition(0, ECoeWinPriorityFep);
    TPoint fepControlPos(0, 0);

    SetExtent(fepControlPos, TSize(0,0));
    window.SetExtent(fepControlPos, TSize(0,0));
    window.SetNonFading(ETrue);
    ((CCoeAppUi*)iEikonEnv->AppUi())->AddToStackL(this,
        ECoeStackPriorityFep, ECoeStackFlagSharable|ECoeStackFlagRefusesFocus);
  }
  virtual ~KeyPressWalkTriggerer() {
    ((CCoeAppUi*)iEikonEnv->AppUi())->RemoveFromStack(this);
  }
  virtual TKeyResponse OfferKeyEventL(const TKeyEvent& /* aKeyEvent */,
                              TEventCode /* aEventCode */) {
    walk_->OnTrigger();
    return EKeyWasNotConsumed;
  }

 private:
  TriggerInterface* walk_;
  LoggingState* logger_;
};

class PeriodicTriggerer : public CActive {
 public:
  PeriodicTriggerer(TriggerInterface* walk, LoggingState* logger)
      : CActive(CActive::EPriorityIdle) {
    walk_ = walk;
    logger_ = logger;
    CActiveScheduler::Add(this);
  }
  virtual ~PeriodicTriggerer() {
    Cancel();
    if (timer_created_) {
      timer_.Close();
    }
  }
  void After(int seconds) {
    if (!timer_created_) {
      User::LeaveIfError(timer_.CreateLocal());
      timer_created_ = true;
    }
    seconds_ = seconds;
    Cancel();
    timer_.After(iStatus, seconds_ * 1000 * 1000);
    SetActive();
  }
 private:
  virtual void DoCancel() {
    timer_.Cancel();
  }
  virtual void RunL() {
    timer_.After(iStatus, seconds_ * 1000 * 1000);
    SetActive();
    walk_->OnTrigger();
  }
  virtual void HandleLosingForeground() {}
  RTimer timer_;
  bool timer_created_;
  TriggerInterface* walk_;
  LoggingState* logger_;
  int seconds_;
};

TtsController::~TtsController() {
  LoggingState::TeardownTls();
  if (original_gc_) {
    Mem::Copy(original_gc_, original_gc_vtbl_, 4);
  }
  delete triggerer_;
  delete key_triggerer_;
  delete periodic_triggerer_;
  delete walker_;
  delete async_trigger_;
  delete app_reader_;

}

void TtsController::ConstructL() {
  LoggingState::SetupTls();

  RProcess me;
  me.Open(me.Id());
  is_phone_ =
      me.FileName().CompareF(_L("z:\\sys\\bin\\phone.exe")) == 0;

  async_trigger_ = new (ELeave) AsyncTrigger(this);
  walker_ = new (ELeave) ControlWalker;
  // walker_->TriggerWalk(LoggingState::Get());
  triggerer_ = new (ELeave) ForegroundWalkTriggerer(async_trigger_,
                                                    LoggingState::Get());
  key_triggerer_ = new (ELeave) KeyPressWalkTriggerer(async_trigger_,
                                                      LoggingState::Get());
  key_triggerer_->ConstructL();
  
  // TODO(mikie): make an array of the different reader creation functions
  // and loop through here.
  if (!app_reader_) {
    app_reader_ = new (ELeave) MenuReader;
    if (app_reader_->ForApplication() != me.SecureId()) {
      delete app_reader_;
      app_reader_ = NULL;
    }
  }
  if (!app_reader_) {
    app_reader_ = new (ELeave) PhoneReader;
    if (app_reader_->ForApplication() != me.SecureId()) {
      delete app_reader_;
      app_reader_ = NULL;
    }
  }
  if (!app_reader_) {
    app_reader_ = new (ELeave) ContactsReader;
    if (app_reader_->ForApplication() != me.SecureId()) {
      delete app_reader_;
      app_reader_ = NULL;
    }
  }
  return;

  CCoeEnv* env = CCoeEnv::Static();
  LoggingWindowGc* temp_logging_gc = new (ELeave) LoggingWindowGc(env->ScreenDevice());
  original_gc_ = env->SwapSystemGc(temp_logging_gc);
  Mem::Copy(original_gc_vtbl_, original_gc_, 4);
  Mem::Copy(original_gc_, temp_logging_gc, 4); // replace the vtable
  env->SwapSystemGc(original_gc_);
  delete temp_logging_gc;
}

namespace {
void ReportAppState(LoggingState* logger, const AppState& app) {
  TBuf<128>* buf_p = new TBuf<128>;
  TBuf<128>& buf = *buf_p;

  buf.Append(app.Title().Left(80));
  buf.Append(_L(" uid: "));
  buf.AppendNum(app.AppUid().iUid, EHex);
  buf.Append(_L(" view: "));
  buf.AppendNum(app.ViewUid().iUid, EHex);
  logger->Log(buf);

  buf.Zero();
  buf.Append(_L("tab "));
  buf.AppendNum(app.SelectedTabIndex());
  buf.Append(_L(" of "));
  buf.AppendNum(app.TabCount());
  buf.Append(_L(": "));
  buf.Append(app.SelectedTabText().Left(80));
  logger->Log(buf);

  buf.Zero();
  buf.Append(_L("item "));
  buf.AppendNum(app.SelectedItemIndex());
  buf.Append(_L(" of "));
  buf.AppendNum(app.ItemCount());
  buf.Append(_L(": "));
  buf.Append(app.SelectedItemText().Left(80));
  logger->Log(buf);

  buf.Zero();
  buf.Append(_L("search field: "));
  buf.Append(app.SearchFieldText().Left(80));
  logger->Log(buf);

  buf.Zero();  
  buf.Append(_L("softkeys "));
  buf.Append(app.FirstSoftkey());
  buf.Append(_L(" "));
  buf.Append(app.SecondSoftkey());
  logger->Log(buf);
  
  logger->Log(app.Debug());
  
  delete buf_p;
}
}  // namespace

void TtsController::OnTrigger() {
  if (kDevelopmentMode) {
    // development mode
    walker_->Walk(LoggingState::Get());
  } else {
    if (app_reader_) {
      app_reader_->Read();
      ReportAppState(LoggingState::Get(), app_reader_->State());
    }
  }
}
