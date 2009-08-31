#ifndef REPORTING_H_
#define REPORTING_H_

#include <e32base.h>
#include <flogger.h>

// LoggingState is a primitive data logger that will be replaced with
// a structured reporting mechanism. It uses RFileLogger to log out
// text prefixed with the process name.
class LoggingState {
 public:
  static LoggingState* Get();
  static void SetupTls();
  static void TeardownTls();
  LoggingState();
  ~LoggingState();
  void IncreaseIndent();
  void DecreaseIndent();
  void Log(const TDesC& message);
 private:
  TBuf<20> indent_;
  TBuf<50> process_name_;
  TBuf<128> buffer_;
  RFileLogger logger_;
};

#endif  // REPORTING_H_
