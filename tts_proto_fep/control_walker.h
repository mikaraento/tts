#ifndef CONTROLWALKER_H_
#define CONTROLWALKER_H_

#include <e32base.h>

class LoggingState;

// ControlWalker recursively iterates the application controls, identifying
// known classes.
// As a first step, it just prints the hierarchy and class names via the
// LoggingState.
class ControlWalker : public CActive{
 public:
  ControlWalker();
  ~ControlWalker();
  void TriggerWalk(LoggingState* logger);
 private:
  void RunL();
  void DoCancel();
  void Walk(LoggingState* logger);
  LoggingState* logger_;
};

#endif  // CONTROLWALKER_H_
