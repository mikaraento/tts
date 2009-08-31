#include "fep_proxy.h"

#include <eikenv.h>
#include <fepplugin.h>

class AknFepLoader : public CActive {
 public:
  AknFepLoader() : CActive(CActive::EPriorityStandard) {
    CActiveScheduler::Add(this);
  }
  ~AknFepLoader() {
    Cancel();
    delete akn_fep_;
    delete akn_plugin_;
  }
  void Trigger() {
    TRequestStatus* s = &iStatus;
    User::RequestComplete(s, KErrNone);
    SetActive();
  }
  void DoCancel() {}
  void RunL() {
    const TUid aknfepuid = { 0x101fd65a };
    akn_plugin_ = CCoeFepPlugIn::NewL(aknfepuid);
    CCoeEnv* env = CEikonEnv::Static();
    // the parameters are ignored.
    //CCoeFepParameters* params = CCoeFepParameters::NewLC();
    CCoeFepParameters* params = NULL;
    akn_fep_ = akn_plugin_->NewFepL(*env, *params); 
    //CleanupStack::PopAndDestroy(params);
  }
  CCoeFep* fep() { return akn_fep_; }
  const CCoeFep* fep() const { return akn_fep_; }
  CCoeFepPlugIn* akn_plugin_;
  CCoeFep* akn_fep_;
};

FepProxy::FepProxy(CCoeEnv& aConeEnvironment) :
  CCoeFep(aConeEnvironment) {
}

MFepAttributeStorer* FepProxy::storer() {
  MFepAttributeStorer* storer = akn_loader_->fep();
  return storer;
}

const MFepAttributeStorer* FepProxy::storer() const {
  const MFepAttributeStorer* storer = akn_loader_->fep();
  return storer;
}

void FepProxy::ConstructL(const CCoeFepParameters& aFepParameters) {
  BaseConstructL(aFepParameters);
  akn_loader_ = new (ELeave) AknFepLoader;
  akn_loader_->Trigger();
}

FepProxy::~FepProxy() {
  delete akn_loader_;
}

TInt FepProxy::NumberOfAttributes() const {
  if (!storer()) return 0;
  return storer()->NumberOfAttributes();
}

TUid FepProxy::AttributeAtIndex(TInt index) const {
  if (!storer()) return KNullUid;
  return storer()->AttributeAtIndex(index);
}

void FepProxy::WriteAttributeDataToStreamL(TUid uid,
                                           RWriteStream& stream) const {
  if (!storer()) return;
  storer()->WriteAttributeDataToStreamL(uid, stream);
}

void FepProxy::ReadAttributeDataFromStreamL(TUid uid,
                                            RReadStream& stream) {
  if (!storer()) return;
  storer()->ReadAttributeDataFromStreamL(uid, stream);
}

void FepProxy::HandleChangeInFocus() {
  if (!akn_loader_->fep()) return;
  akn_loader_->fep()->HandleChangeInFocus();
}

void FepProxy::HandleDestructionOfFocusedItem() {
  if (!akn_loader_->fep()) return;
  akn_loader_->fep()->HandleDestructionOfFocusedItem();
}

void FepProxy::HandleGainingForeground() {
  if (!akn_loader_->fep()) return;
  akn_loader_->fep()->HandleGainingForeground();
}

void FepProxy::HandleLosingForeground() {
  if (!akn_loader_->fep()) return;
  akn_loader_->fep()->HandleLosingForeground();
}

void FepProxy::CancelTransaction() {
  if (!akn_loader_->fep()) return;
  akn_loader_->fep()->CancelTransaction();
}

void FepProxy::IsOnHasChangedState() {
  if (!akn_loader_->fep()) return;
  FepProxy* cast_for_access = (FepProxy*)akn_loader_->fep();
  cast_for_access->IsOnHasChangedState();
}

void FepProxy::SynchronouslyExecuteSettingsDialogL(CCoeEnv& env) {
  if (!akn_loader_->akn_plugin_) return;
  akn_loader_->akn_plugin_->SynchronouslyExecuteSettingsDialogL(env);
}
