#ifndef APP_READERS_PHONE_H_
#define APP_READERS_PHONE_H_

#include "app_helper.h"
#include "app_reader.h"
#include "control_tree.h"
#include "rtti.h"

// Reads the Phone app.
class PhoneReader : public CBase, public AppReaderInterface {
 public:
  PhoneReader() {}
  virtual ~PhoneReader() {}
  virtual const TUid& ForApplication() const;
  virtual void Read();
  virtual const AppState& State() const { return app_state_; }

 private:
  AppState app_state_;
  ControlTree control_tree_;
  AppHelper app_helper_;
  SafeTypes safe_;
};

#endif  // APP_READERS_PHONE_H_
