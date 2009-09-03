#include "tts_proto_fep.h"

#include <coeaui.h>
#include <coecntrl.h>
#include <coemain.h>
#include <ecom/ecom.h>
#include <eikenv.h>
#include <fepbase.h>
#include <implementationproxy.h>

#include "control_walker.h"
#include "fep_proxy.h"
#include "logging_window_gc.h"
#include "reporting.h"

// TODO(mikie): move the Triggerer classes to their own file and generalize
// their action.
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

CCoeFepPlugIn* TtsProtoFepPlugin::NewL() {
  // The Phone application doesn't like the indirectly created AKNFEP. The
  // easiest workaround is just to fail in that process: it'll keep the
  // AKNFEP it already has.
  //
  // Things that I tried that didn't work:
  //  - directly hand out the CCoeFepPlugin from the AKNFEP factory function
  //  - use our own CCoeFep-derived class that delegates to the one from AKNFEP.
  //  - call REComSession::FinalClose() judiciously to make sure the AKNFEP
  //    has been unloaded before we load it again.
  //  - call CancelTransaction() on the AKNFEP CCoeFep in case it has some
  //    lingering state.
  RProcess me;
  me.Open(me.Id());
  if (me.FileName().CompareF(_L("z:\\sys\\bin\\phone.exe")) == 0) {
    me.Close();
    User::Leave(KErrAlreadyExists);
  }
  /*
  if (me.FileName().CompareF(_L("z:\\sys\\bin\\phonebook.exe")) != 0) {
    me.Close();
    User::Leave(KErrAlreadyExists);
  }
  if (me.FileName().CompareF(_L("z:\\sys\\bin\\menu.exe")) != 0) {
    me.Close();
    User::Leave(KErrAlreadyExists);
  }*/
  me.Close();

  TtsProtoFepPlugin* self = new (ELeave) TtsProtoFepPlugin;
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(self);
  return self;
}

TtsProtoFepPlugin::~TtsProtoFepPlugin() {
  // TODO(mikie): installing another FEP hangs the phone - see if that can
  // be helped.
  delete akn_plugin_;
  LoggingState::TeardownTls();
  if (original_gc_) {
    Mem::Copy(original_gc_, original_gc_vtbl_, 4);
  }
  delete triggerer_;
  delete key_triggerer_;
  delete walker_;
  delete async_trigger_;
}

CCoeFep* TtsProtoFepPlugin::NewFepL(CCoeEnv& aCoeEnv,
                                    const CCoeFepParameters& aFepParameters) {
  async_trigger_ = new (ELeave) AsyncTrigger(this);
  walker_ = new (ELeave) ControlWalker;
  // walker_->TriggerWalk(LoggingState::Get());
  triggerer_ = new (ELeave) ForegroundWalkTriggerer(async_trigger_,
                                                    LoggingState::Get());
  key_triggerer_ = new (ELeave) KeyPressWalkTriggerer(async_trigger_,
                                                      LoggingState::Get());
  key_triggerer_->ConstructL();
#if 0
  const TUid aknfepuid = { 0x101fd65a };
  akn_plugin_ = CCoeFepPlugIn::NewL(aknfepuid);
  return akn_plugin_->NewFepL(aCoeEnv, aFepParameters);
#else
  fep_proxy_ = new (ELeave) FepProxy(aCoeEnv);
  fep_proxy_->ConstructL(aFepParameters);
  return fep_proxy_;
#endif
}


void TtsProtoFepPlugin::SynchronouslyExecuteSettingsDialogL(
    CCoeEnv& aConeEnvironment) {
  if (fep_proxy_) {
    fep_proxy_->SynchronouslyExecuteSettingsDialogL(aConeEnvironment);
  } else if (akn_plugin_) {
    akn_plugin_->SynchronouslyExecuteSettingsDialogL(aConeEnvironment);
  }
}

TtsProtoFepPlugin::TtsProtoFepPlugin() {
}

void TtsProtoFepPlugin::ConstructL() {
  LoggingState::SetupTls();

  return;

  CCoeEnv* env = CCoeEnv::Static();
  LoggingWindowGc* temp_logging_gc = new (ELeave) LoggingWindowGc(env->ScreenDevice());
  original_gc_ = env->SwapSystemGc(temp_logging_gc);
  Mem::Copy(original_gc_vtbl_, original_gc_, 4);
  Mem::Copy(original_gc_, temp_logging_gc, 4); // replace the vtable
  env->SwapSystemGc(original_gc_);
  delete temp_logging_gc;
}

void TtsProtoFepPlugin::OnTrigger() {
  walker_->Walk(LoggingState::Get());
}
                       
//
const TImplementationProxy kImplementationTable[] = {
        IMPLEMENTATION_PROXY_ENTRY(0x20006E90, TtsProtoFepPlugin::NewL) };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(
    TInt& table_count_out) {
  table_count_out = sizeof(kImplementationTable) / sizeof(TImplementationProxy);
  return kImplementationTable;
}
