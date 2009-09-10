#ifndef MYFEPPLUGIN_H_
#define MYFEPPLUGIN_H_

#include <fepplugin.h> // CCoeFepPlugIn

class TtsController;
class FepProxy;

// TtsProtoFepPlugin is the bootstrap class for the TTS FEP: it gets loaded
// by CCoeEnv::InstallFepL(), it instantiates an AKNFEP CCoeFep and starts
// all the text introspection code.
class TtsProtoFepPlugin: public CCoeFepPlugIn {
 public:
  static CCoeFepPlugIn* NewL();

 private:
  TtsProtoFepPlugin();
  void ConstructL();
  virtual ~TtsProtoFepPlugin();
  virtual CCoeFep* NewFepL(CCoeEnv& aCoeEnv,
                           const CCoeFepParameters& aFepParameters);
  virtual void SynchronouslyExecuteSettingsDialogL(CCoeEnv& aCoeEnv);

  CCoeFepPlugIn* akn_plugin_;
  FepProxy* fep_proxy_;
  TtsController* controller_;
};

#endif  // MYFEPPLUGIN_H_
