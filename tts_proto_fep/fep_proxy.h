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
  // The following three functions are deprecated but need to be defined when
  // compiling for < FP2.
  virtual void OfferKeyEventL(TEventResponse& /*aEventResponse*/, const TKeyEvent& /*aKeyEvent*/, TEventCode /*aEventCode*/){};
  virtual void OfferPointerEventL(TEventResponse& /*aEventResponse*/, const TPointerEvent& /*aPointerEvent*/, const CCoeControl* /*aWindowOwningControl*/){};
  virtual void OfferPointerBufferReadyEventL(TEventResponse& /*aEventResponse*/, const CCoeControl* /*aWindowOwningControl*/){};

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
