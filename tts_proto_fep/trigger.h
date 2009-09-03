#ifndef TRIGGER_H_
#define TRIGGER_H_

#include <e32base.h>

// For simple callbacks on events.
class TriggerInterface {
 public:
  virtual void OnTrigger() = 0;
};

// Wrap one callback so that it'll get run asynchronously.
class AsyncTrigger : public CActive, public TriggerInterface {
 public:
  AsyncTrigger(TriggerInterface* inner);
  ~AsyncTrigger();
  virtual void OnTrigger();
 private:
  void RunL();
  void DoCancel();
  TriggerInterface* inner_;
};

#endif  // TRIGGER_H_
