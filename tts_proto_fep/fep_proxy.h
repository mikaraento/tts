#ifndef FEP_PROXY_H_
#define FEP_PROXY_H_

#include <fepbase.h>

class AknFepLoader;

class FepProxy: public CCoeFep {
public:
  FepProxy(CCoeEnv& aConeEnvironment);
  void ConstructL(const CCoeFepParameters& aFepParameters);
  virtual ~FepProxy();
  void SynchronouslyExecuteSettingsDialogL(CCoeEnv& env);

 private:
  // from CCoeFep
  virtual void CancelTransaction();
  virtual void IsOnHasChangedState();

  // from MCoeFocusObserver
  virtual void HandleChangeInFocus();
  virtual void HandleDestructionOfFocusedItem();

  // from MCoeForegroundObserver
  virtual void HandleGainingForeground();
  virtual void HandleLosingForeground();

  // from MFepAttributeStorer (via CCoeFep)
  virtual TInt NumberOfAttributes() const;
  virtual TUid AttributeAtIndex(TInt aIndex) const;
  virtual void WriteAttributeDataToStreamL(TUid aAttributeUid,
                                           RWriteStream& aStream) const;
  virtual void ReadAttributeDataFromStreamL(TUid aAttributeUid,
                                            RReadStream& aStream);

 private:
  MFepAttributeStorer* storer();
  const MFepAttributeStorer* storer() const;
  AknFepLoader* akn_loader_;
  CCoeEnv* env_;
};

#endif

// End of file
