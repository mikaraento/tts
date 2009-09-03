#include "app_readers/menu.h"

#include <aknappui.h>
#include <akntitle.h>
#include <eikenv.h>
#include <eiklbx.h>

namespace {
const TUid kMenuUid = { 0x101f4cd2 };
}  // namespace

const TUid& MenuReader::ForApplication() const {
  return kMenuUid;
}

void MenuReader::GetMainView() {
  if (main_view_) return;
  const TRect client_rect = control_tree_.AppUi()->ClientRect();
  control_tree_.RefreshWindowList();
  const ControlTree::ControlArray& controls = control_tree_.WindowList();
  for (int i = 0; i < controls.Count(); ++i) {
    CCoeControl* control = controls[i];
    if (control->Rect() == client_rect) {
      main_view_ = control;
      return;
    }
  }
}

void MenuReader::Read() {
  app_state_.Reset();
  CAknTitlePane* title = control_tree_.TitlePane();
  if (title) app_state_.SetTitle(*title->Text());
  
  if (control_tree_.AppUi()->IsDisplayingMenuOrDialog()) {
    app_state_.SetIsShowingMenuOrPopup(ETrue);
    return;
  }
  CEikListBox* listbox = NULL;
  if (main_view_) {
    if (main_view_->CountComponentControls() > 0) {
      CCoeControl* container = main_view_->ComponentControl(0);
      if (container->CountComponentControls() > 0) {
        listbox = (CEikListBox*)container->ComponentControl(0);
      }
    }
  }
  if (listbox) {
    MListBoxModel* model = listbox->Model();
    app_state_.SetItemCount(model->NumberOfItems());
    if (model->NumberOfItems() > 0) {
      const MDesCArray* array = model->MatchableTextArray();
      app_state_.SetSelectedItemIndex(listbox->CurrentItemIndex());
      app_state_.SetItemCount(model->NumberOfItems());
      const TDesC& text = array->MdcaPoint(listbox->CurrentItemIndex());
      app_state_.SetSelectedItemText(text);
    }
  }
}

