#ifndef CONTROLWALKER_H_
#define CONTROLWALKER_H_

#include <e32base.h>

class LoggingState;

// ControlWalker recursively iterates the application controls, identifying
// known classes.
// As a first step, it just prints the hierarchy and class names via the
// LoggingState.
class ControlWalker : public CBase {
 public:
  ControlWalker();
  ~ControlWalker();
  void Walk(LoggingState* logger);
 private:
  LoggingState* logger_;
};

#endif  // CONTROLWALKER_H_
