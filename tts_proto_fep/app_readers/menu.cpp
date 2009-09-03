#include "app_readers/menu.h"

// There's lots of generic state reading (softkeys, menu) here right now -
// it'll get factored out to be used by the ContactsReader too.

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

// Although CEikMenuPane declares an IMPORT_C CascadeMenuPane() function
// it's not actually in the LIBs.
class CEikMenuPaneExtension {
 public:
  static inline CEikMenuPane* CascadeMenuPane(CEikMenuPane* pane) {
    return pane->iCascadeMenuPane;
  }
};

void MenuReader::GetMainView() {
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

void MenuReader::Read() {
  app_state_.Reset();
  CAknTitlePane* title = control_tree_.TitlePane();
  if (title) app_state_.SetTitle(*title->Text());

  TVwsViewId view_id;
  TInt view_found = control_tree_.AppUi()->GetActiveViewId(view_id);
  if (view_found != KErrNotFound) {
    app_state_.SetAppUid(view_id.iAppUid);
    app_state_.SetViewUid(view_id.iViewUid);
  }

  // From
  // http://wiki.forum.nokia.com/index.php/TSS000675_-_Retrieving_text_for_softkey_labels
  CEikButtonGroupContainer* cba = control_tree_.Cba();
  if (cba) {
    MEikButtonGroup* buttonGroup = cba->ButtonGroup();
    for (int pos = 0; pos < 3; ++pos) {
      const TInt cmdId = buttonGroup->CommandId(pos);
      CCoeControl* button = buttonGroup->GroupControlById(cmdId);
      if (button && buttonGroup->IsCommandVisible(cmdId)) {
        CEikLabel* label =
                static_cast<CEikLabel*> (button->ComponentControl(0));
        const TDesC* txt = label->Text();
        if (pos == 0) {
          app_state_.SetFirstSoftkey(*txt);
        } else {
          app_state_.SetSecondSoftkey(*txt);          
        }
      }
    }
  }

  if (control_tree_.AppUi()->IsDisplayingMenuOrDialog()) {
    // Bleh - this doesn't seem to work.
    app_state_.SetIsShowingMenuOrPopup(ETrue);
    return;
  }
  CEikMenuBar* menu = control_tree_.MenuBar();
  if (menu && menu->IsDisplayed()) {
    app_state_.SetIsShowingMenuOrPopup(ETrue);
    CEikMenuPane* pane = menu->MenuPane();
    if (pane) {
      CEikMenuPane* cascade = pane;
      while (cascade) {
        pane = cascade;
        cascade = CEikMenuPaneExtension::CascadeMenuPane(pane);
      }
      app_state_.SetItemCount(pane->NumberOfItemsInPane());
      app_state_.SetSelectedItemIndex(pane->SelectedItem());
      const CEikMenuPaneItem::SData& data = pane->ItemDataByIndexL(
          pane->SelectedItem());
      app_state_.SetSelectedItemText(data.iText);
    }
    return;
  }
  
  if (view_id.iViewUid.iUid != 1) {
    // Don't recognize this view, bail out.
    return;
  }

  CEikListBox* listbox = NULL;
  GetMainView();
  if (main_view_) {
    if (main_view_->CountComponentControls() > 0) {
      // This shows the brittleness of the code: the control layout is
      // different on the (3.0 MR) emulator than on the device. May need
      // tuning for different firmware versions of the same device too.
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

