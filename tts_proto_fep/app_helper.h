#ifndef APP_HELPER_H_
#define APP_HELPER_H_

#include <vwsdef.h>

class AppState;
class CEikButtonGroupContainer;
class CEikMenuPane;
class ControlTree;

// AppHelper contains helper methods for reading the state of any application.
class AppHelper {
 public:
  void ReadMenu(CEikMenuPane* pane, AppState* app_state);
  void ReadTitle(ControlTree* control_tree, AppState* app_state);
  TVwsViewId ReadView(ControlTree* control_tree, AppState* app_state);
  CEikButtonGroupContainer* ReadCba(ControlTree* control_tree,
                                    AppState* app_state);
};

#endif  // APP_HELPER_H_
