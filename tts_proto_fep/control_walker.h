#ifndef CONTROLWALKER_H_
#define CONTROLWALKER_H_

class LoggingState;

// ControlWalker recursively iterates the application controls, identifying
// known classes.
// As a first step, it just prints the hierarchy and class names via the
// LoggingState.
class ControlWalker {
 public:
  ControlWalker();
  ~ControlWalker();
  void Walk(LoggingState* logger);
};

#endif  // CONTROLWALKER_H_
