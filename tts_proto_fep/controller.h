#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <e32base.h>

#include "trigger.h"

class AppReaderInterface;
class AsyncTrigger;
class ControlWalker;
class CWindowGc;
class ForegroundWalkTriggerer;
class KeyPressWalkTriggerer;
class PeriodicTriggerer;

// TtsController creates all the necessary objects for introspecting the
// screen and logging the results.
class TtsController : public CBase, public TriggerInterface {
 public:
  TtsController() {}
  virtual ~TtsController();
  void ConstructL();
  virtual void OnTrigger();
 private: 
  AsyncTrigger* async_trigger_;
  ControlWalker* walker_;
  ForegroundWalkTriggerer* triggerer_;
  KeyPressWalkTriggerer* key_triggerer_;
  PeriodicTriggerer* periodic_triggerer_;
  AppReaderInterface* app_reader_;
  CWindowGc* original_gc_;
  char original_gc_vtbl_[4];
  bool is_phone_;
};

#endif  // CONTROLLER_H_
