#include "app_readers/menu.h"

// The Menu app is odd. The main container cannot be found through the
// TopFocusedControl nor the control stack. The menus don't come through
// the AppUi's or current view's MenuBar().
// I might be missing something here :-(

// We find the grid/list by looking for a window that has a control that
// fills the client rect and picking a known child.
// We find the current menu by looking through the windows and recognizing
// a CEikMenuPane. Only do this if we have the Select/Cancel softkeys that
// indicate a menu.

#include <aknappui.h>
#include <akngrid.h>
#include <akntitle.h>
#include <aknview.h>
#include <eikenv.h>
#include <eiklbx.h>

namespace {
const TUid kMenuUid = { 0x101f4cd2 };
}  // namespace

const TUid& MenuReader::ForApplication() const {
  return kMenuUid;
}

class CEikMenuPaneExtension {
 public:
  static inline CEikMenuPane* Owner(CEikMenuPane* pane) {
    return pane->iOwner;
  }
};

void MenuReader::GetMainView() {
  // We assume that the main container never goes away. May be unsafe.
  if (main_view_) return;
  TRect client_rect;
  TSize client_size;
  if (control_tree_.View()) {
    client_rect = control_tree_.View()->ClientRect();
    client_size = client_rect.Size();
  } else {
    client_rect = control_tree_.AppUi()->ClientRect();
    client_size = client_rect.Size();
    CCoeControl* cba = control_tree_.Cba();
    if (cba) {
      client_size.iHeight -= cba->Size().iHeight;
    }
  }
  control_tree_.RefreshWindowList();
  const ControlTree::ControlArray& controls = control_tree_.WindowList();
  for (int i = 0; i < controls.Count(); ++i) {
    CCoeControl* control = controls[i];
    TSize control_size = control->Rect().Size();
    if (client_size == control_size) {
      main_view_ = control;
      return;
    }
  }
}

void MenuReader::ReadMenuState(CEikMenuPane* pane) {
  if (pane) {
    // We walk the menu hierarchy up as we may have found any odd pane
    // from the windows. This is necessary so that caching the pane
    // works even with cascaded menus.
    CEikMenuPane* owner = pane;
    pane = NULL;
    while (owner && owner != pane) {
      pane = owner;
      owner = CEikMenuPaneExtension::Owner(pane);
    }
    latest_menu_pane_ = pane;
    app_helper_.ReadMenu(pane, &app_state_);
  }
}

void MenuReader::Read() {
  app_state_.Reset();
  app_helper_.ReadTitle(&control_tree_, &app_state_);
  const TVwsViewId view_id = app_helper_.ReadView(&control_tree_, &app_state_);
  CEikButtonGroupContainer* cba =
      app_helper_.ReadCba(&control_tree_, &app_state_);

#ifndef __WINS__
  // Can't find the menu this way in the Menu app on-device.
  if (view_id.iViewUid.iUid != 1 || view_id.iAppUid != ForApplication())
#endif  // !__WINS__
  {
    CEikMenuBar* menu = control_tree_.MenuBar();
    if (0) {
      // We just see menu = 0 on-device.
      TBuf<100> debug;
      debug.Append(_L("menu = "));
      debug.AppendNum((TUint32)menu, EHex);
      if (menu) {
        debug.Append(_L(" isfocused = "));
        debug.AppendNum((TInt32)menu->IsFocused());
        debug.Append(_L(" is_visible = "));
        debug.AppendNum((TInt32)menu->IsVisible());
        debug.Append(_L(" is_displayed = "));
        debug.AppendNum((TInt32)menu->IsDisplayed());
      }
      app_state_.SetDebug(debug);
    }
    if (menu && menu->IsFocused()) {
      CEikMenuPane* pane = menu->MenuPane();
      ReadMenuState(pane);
      if (pane) {
        return;
      }
    }
  }
  
  if (view_id.iViewUid.iUid != 1 || view_id.iAppUid != ForApplication()) {
    // Don't recognize this view, bail out.
    return;
  }
  
#ifndef __WINS__
  TInt first_command = 0;
  TInt second_command = 0;
  if (cba) {
    MEikButtonGroup* buttonGroup = cba->ButtonGroup();
    for (int pos = 0; pos < 3; ++pos) {
      const TInt cmd_id = buttonGroup->CommandId(pos);
      CCoeControl* button = buttonGroup->GroupControlById(cmd_id);
      if (button && buttonGroup->IsCommandVisible(cmd_id)) {
        if (pos == 0) {
          first_command = cmd_id;
        } else {
          second_command = cmd_id;
        }
      }
    }
  }

  if (first_command == EAknSoftkeySelect &&
      second_command == EAknSoftkeyCancel) {
    // We are probably showing a menu. Let's root around the windows
    // to find it.
    if (!latest_menu_pane_) {
      // This optimization may not be safe. Let's look and see.
      // Since RefreshWindowList() is expensive we don't do it if the
      // softkeys have stayed the same since the last time we found a menu.
      control_tree_.RefreshWindowList();
      const ControlTree::ControlArray& list = control_tree_.WindowList();
      for (int i = 0; i < list.Count(); ++i) {
        CCoeControl* control = list[i];
        CEikMenuPane* pane = safe_.IsEikMenuPane(control);
        if (pane) {
          ReadMenuState(pane);
          break;
        }
      }
    } else {
      ReadMenuState(latest_menu_pane_);
    }
    return;
  } else {
    latest_menu_pane_ = NULL;
  }
#endif  // !__WINS__

  CEikListBox* listbox = NULL;
  GetMainView();
  if (main_view_) {
    if (main_view_->CountComponentControls() > 0) {
      // This shows the brittleness of the code: the control layout is
      // different on the (3.0 MR) emulator than on the device. May need
      // tuning for different firmware versions of the same device too.
      // Getting this wrong will crash. Could try to play safer by using
      // the recognition from UnsafeTypes.
#ifdef __WINS__
      CCoeControl* container = main_view_;
#else
      CCoeControl* container = main_view_->ComponentControl(0);
#endif
      if (container->CountComponentControls() > 0) {
        listbox = (CEikListBox*)container->ComponentControl(0);
      }
    }
  }
  if (listbox) {
    // This works even if the menu is in list mode as lists and grids are
    // almost the same.
    CAknGrid* grid = (CAknGrid*)listbox;
    CTextListBoxModel* model = grid->Model();
    app_state_.SetItemCount(model->NumberOfItems());
    if (model->NumberOfItems() > 0) {
      app_state_.SetSelectedItemIndex(listbox->CurrentItemIndex());
      app_state_.SetItemCount(model->NumberOfItems());
      if (model->NumberOfItems() > listbox->CurrentItemIndex()) {
        app_state_.SetSelectedItemText(model->ItemText(
            listbox->CurrentItemIndex()));
      }
    }
  }
}

