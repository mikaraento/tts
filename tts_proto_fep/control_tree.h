#ifndef CONTROL_TREE_H_
#define CONTROL_TREE_H_

#include <e32base.h>
#include <e32cmn.h>

class CAknAppUi;
class CAknTabGroup;
class CAknTitlePane;
class CAknView;
class CCoeControl;
class CEikButtonGroupContainer;
class CEikMenuBar;
class CEikonEnv;

// ControlTree provides access to the controls of this thread through
// various means.
class ControlTree : public CBase {
 public:
  ControlTree();
  virtual ~ControlTree();
  CEikonEnv* Env();
  CAknAppUi* AppUi();
  // All of the accessors below may return NULL or an empty list.
  CAknTitlePane* TitlePane();
  CEikButtonGroupContainer* Cba();
  CAknView* View();
  CEikMenuBar* MenuBar();
  CAknTabGroup* TabGroup();
  typedef RArray<CCoeControl*> ControlArray;
  // Return CCoeAppUi::TopFocusedControl
  CCoeControl* TopFocusedControl();
  // Return all the controls on CCoeAppUi::iStack (the control stack).
  const ControlArray& ControlStack();
  void RefreshControlStack();
  // Return all the controls bound to top-level windows.
  const ControlArray& WindowList();
  // Note that refreshing the window list is very expensive as it flushes
  // the window server command queue several times.
  void RefreshWindowList();

 private:
  void WalkWindows(TUint32 handle, bool forward);

  ControlArray stack_;
  ControlArray windows_;
  CAknAppUi* app_ui_;
  CEikonEnv* eikon_env_;
};

#endif  // CONTROL_TREE_H_
