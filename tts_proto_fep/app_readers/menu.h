#ifndef APP_READERS_MENU_H_
#define APP_READERS_MENU_H_

#include "app_reader.h"
#include "control_tree.h"

// Reads the main menu.
class MenuReader : public CBase, public AppReaderInterface {
 public:
  MenuReader() {}
  virtual ~MenuReader() {}
  virtual const TUid& ForApplication() const;
  virtual void Read();
  virtual const AppState& State() const { return app_state_; }
 private:
  void GetMainView();
  // Owned
  AppState app_state_;
  ControlTree control_tree_;
  // Not owned.
  CCoeControl* main_view_;
};
#endif  // APP_READERS_MENU_H_
