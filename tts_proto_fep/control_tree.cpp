// This file will contain the cleaned-up version of all the control walking
// from control_walker.cpp

#include "control_tree.h"

#include "modifed_system_include/aknview.h"
#include "modifed_system_include/coeaui.h"

#include <aknappui.h>
#include <akntitle.h>
#include <aknviewappui.h>
#include <eikbtgpc.h>
#include <eikenv.h>
#include <eikmenub.h>
#include <eikspane.h>


// CCoeControl declares an internal class called CCoeRedrawer as a friend.
// We can declare our own class with that name as a convenient way to access
// private parts of CCoeControl.
class CCoeRedrawer {
 public:
  inline static RWindow* Window(CCoeControl* ctrl) { return &ctrl->Window(); }
};

ControlTree::ControlTree() {
  // These will never change during the lifetime of the app.
  eikon_env_ = CEikonEnv::Static();
  app_ui_ = (CAknAppUi*)eikon_env_->AppUi();
}

ControlTree::~ControlTree() {
  stack_.Close();
  windows_.Close();
}

CEikonEnv* ControlTree::Env() {
  return eikon_env_;
}

CAknTitlePane* ControlTree::TitlePane() {
  CEikStatusPane* sp = eikon_env_->AppUiFactory()->StatusPane();
  CAknTitlePane* title_pane = NULL;
  if (sp) {
    title_pane = (CAknTitlePane*)
        sp->ControlL(TUid::Uid(EEikStatusPaneUidTitle));
  }
  return title_pane;  
}

CAknView* ControlTree::View() {
  TVwsViewId view_id;
  TInt view_found = app_ui_->GetActiveViewId(view_id);
  if (view_found != KErrNotFound) {
    if (eikon_env_->AppUiFactory()->Cba()) {
      // Not view-based
      return NULL;
    }
    CAknViewAppUi* viewappui = (CAknViewAppUi*)app_ui_;
    CAknView* view = viewappui->View(view_id.iViewUid);
    return view;
  }
  return NULL;
}

CEikButtonGroupContainer* ControlTree::Cba() {
  CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
  if (cba) return cba;
  cba = eikon_env_->AppUiFactory()->Cba();
  if (cba) return cba;
  CAknView* view = View();
  if (view) cba = view->iCba;
  return cba;
}

CAknAppUi* ControlTree::AppUi() {
  return app_ui_;
}

CEikMenuBar* ControlTree::MenuBar() {
  CEikMenuBar* menu = NULL;
  CAknView* view = View();
  if (view) {
    // I have no idea why view->MenuBar() doesn't give us the menu.
    menu = view->iMenu;
  }
  if (!menu) menu = eikon_env_->AppUiFactory()->MenuBar();
  return menu;
}

CCoeControl* ControlTree::TopFocusedControl() {
  return app_ui_->TopFocusedControl();
}

const ControlTree::ControlArray& ControlTree::ControlStack() {
  return stack_;
}

// From ER5 sources. On SOS 9 the length of an element is 16 instead of 12
// so I've added an iUnknown field.
struct SStackedControl {
  CCoeControl* iControl;
  TInt iPriority;
  TInt iFlags;
  TInt iUnknown;
};

class CCoeControlStack: public CArrayFixFlat<SStackedControl> {
 public:
  inline CCoeControlStack() :
    CArrayFixFlat<SStackedControl> (2) {
  } // granularity of two
  TKeyResponse OfferKeyL(const TKeyEvent& aKeyEvent, TEventCode aType);
 public:
  TInt iKeyIndex;
};

void ControlTree::RefreshControlStack() {
  stack_.Reset();
  CCoeAppUiAccess* appui = (CCoeAppUiAccess*)app_ui_;
  CCoeControlStack* stack = appui->iStack;
  for (int i = 0; i < stack->Count(); ++i) {
    SStackedControl s = stack->At(i);
    stack_.AppendL(s.iControl);
  }  
}

const ControlTree::ControlArray& ControlTree::WindowList() {
  return windows_;
}

void ControlTree::WalkWindows(TUint32 handle, bool forward) {
  // We walk the list twice, only push the starting point when walking
  // forwards.
  bool push_to_list = forward;
  while (handle) {
    // With CEikonEnv the RWindow handle must always be a pointer to
    // a CCoeControl (see http://mikie.iki.fi/wordpress/?p=20).
    CCoeControl* control = (CCoeControl*)handle;
    if (push_to_list) {
      windows_.AppendL(control);
    }
    RWindow* window = CCoeRedrawer::Window(control);
    if (!window) {
      handle = 0;
    } else {
      if (forward) {
        handle = window->NextSibling();
      } else {
        handle = window->PrevSibling();
      }
    }
    push_to_list = true;
  }
}

void ControlTree::RefreshWindowList() {
  windows_.Reset();
  CCoeControl* control_from = Cba();
  if (!control_from) control_from = TitlePane();
  while (control_from) {
    RWindow* window = CCoeRedrawer::Window(control_from);
    if (window) {
      const TUint32 handle = (TUint32)control_from;
      WalkWindows(handle, false);
      WalkWindows(handle, true);
      break;
    }
    control_from = control_from->Parent();
  }
}
