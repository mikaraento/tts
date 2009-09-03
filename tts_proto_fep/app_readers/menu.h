#ifndef APP_READERS_MENU_H_
#define APP_READERS_MENU_H_

#include "app_reader.h"
#include "control_tree.h"
#include "rtti.h"

class CEikMenuPane;

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
  void ReadMenuState(CEikMenuPane* pane);
  // Owned
  AppState app_state_;
  ControlTree control_tree_;
  // Not owned.
  CCoeControl* main_view_;
  CEikMenuPane* latest_menu_pane_;
  SafeTypes safe_;
};

#endif  // APP_READERS_MENU_H_
