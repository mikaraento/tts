// MyFepPlugin.h

#ifndef __MYFEPPLUGIN_H__
#define __MYFEPPLUGIN_H__

#include <fepplugin.h> // CCoeFepPlugIn
class CWindowGc;

class TtsProtoFepPlugin: public CCoeFepPlugIn {
 public:
  static CCoeFepPlugIn* NewL();
  ~TtsProtoFepPlugin();

  // CCoeFepPlugin
  CCoeFep* NewFepL(CCoeEnv& aCoeEnv, const CCoeFepParameters& aFepParameters);
  void SynchronouslyExecuteSettingsDialogL(CCoeEnv& aCoeEnv);

 private:
  TtsProtoFepPlugin();
  void ConstructL();

  CCoeEnv* iCoeEnv;
  CCoeFepPlugIn* akn_plugin_;
  TUid akn_ecom_dtor_uid_;
  CWindowGc* original_gc_;
  char original_gc_vtbl_[4];
};

#endif // __MYFEPPLUGIN_H__
