#ifndef MYFEPPLUGIN_H_
#define MYFEPPLUGIN_H_

#include <fepplugin.h> // CCoeFepPlugIn

#include "trigger.h"

// Note that although installing a new version of the FEP may succeed you may
// still actually get the old one if it's not been fully unloaded. The sure
// way to get the new one is to reboot the phone and only install with FepSetup
// after that.

class AppReaderInterface;
class AsyncTrigger;
class ControlWalker;
class CWindowGc;
class FepProxy;
class ForegroundWalkTriggerer;
class KeyPressWalkTriggerer;

// TtsProtoFepPlugin is the bootstrap class for the TTS FEP: it gets loaded
// by CCoeEnv::InstallFepL(), it instantiates an AKNFEP CCoeFep and starts
// all the text introspection code.
class TtsProtoFepPlugin: public CCoeFepPlugIn, public TriggerInterface {
 public:
  static CCoeFepPlugIn* NewL();

 private:
  TtsProtoFepPlugin();
  void ConstructL();
  virtual ~TtsProtoFepPlugin();
  virtual CCoeFep* NewFepL(CCoeEnv& aCoeEnv,
                           const CCoeFepParameters& aFepParameters);
  virtual void SynchronouslyExecuteSettingsDialogL(CCoeEnv& aCoeEnv);
  
  virtual void OnTrigger();

  CCoeFepPlugIn* akn_plugin_;
  FepProxy* fep_proxy_;
  TUid akn_ecom_dtor_uid_;
  CWindowGc* original_gc_;
  char original_gc_vtbl_[4];
  AsyncTrigger* async_trigger_;
  ControlWalker* walker_;
  ForegroundWalkTriggerer* triggerer_;
  KeyPressWalkTriggerer* key_triggerer_;
  AppReaderInterface* app_reader_;
};

#endif  // MYFEPPLUGIN_H_
