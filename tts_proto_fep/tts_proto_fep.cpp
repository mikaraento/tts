#include "tts_proto_fep.h"

#include <ecom/ecom.h>
#include <fepbase.h>
#include <implementationproxy.h>

#include "controller.h"
#include "fep_proxy.h"
#include "reporting.h"

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
    // In the phone process we want to run the TTS code even though we are not
    // the FEP. To do that we need to: 1) keep the dll loaded and 2) start
    // the TTS code.
    // This will keep the dll loaded even if the FEP is uninstalled. An OOB
    // mechanism will be needed to tell this unload if unloading support
    // is needed.
    // With this code the only way to unload the FEP fully and access the
    // log file is to reboot the phone.
    if (LoggingState::Get()) {
      // We are already loaded in this process, just get out.
      me.Close();
      User::Leave(KErrAlreadyExists);
    }
    {
      RLibrary l;
      const TInt err = l.Load(_L("tts_proto_fep_20006e90.dll"));
      if (err != KErrNone) {
        User::Panic(_L("TTS-FEP-1"), err);
      }
      // Purposefully leak the handle - would need to keep track if unload
      // was wanted.
    }
    {
      TtsController* controller = new (ELeave) TtsController;
      controller->ConstructL();
      // Purposefully leak the object - would need to keep track if unload
      // was wanted.
    }
    me.Close();
    User::Leave(KErrAlreadyExists);
  }
  me.Close();

  TtsProtoFepPlugin* self = new (ELeave) TtsProtoFepPlugin;
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(self);
  return self;
}

TtsProtoFepPlugin::~TtsProtoFepPlugin() {
  delete controller_;
  delete akn_plugin_;
}

CCoeFep* TtsProtoFepPlugin::NewFepL(CCoeEnv& aCoeEnv,
                                    const CCoeFepParameters& aFepParameters) {
  controller_ = new (ELeave) TtsController;
  controller_->ConstructL();
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
}

//
const TImplementationProxy kImplementationTable[] = {
        IMPLEMENTATION_PROXY_ENTRY(0x20006E90, TtsProtoFepPlugin::NewL) };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(
    TInt& table_count_out) {
  table_count_out = sizeof(kImplementationTable) / sizeof(TImplementationProxy);
  return kImplementationTable;
}
