#include "app_readers/contacts.h"

#include "modifed_system_include/akntabgrp.h"

#include <akntabgrp.h>
#include <aknsfld.h>
#include <eiklabel.h>
#include <eiklbx.h>
#include <eikmenub.h>

namespace {
const TUid kContactsUid = { 0x101f4cce };
}  // namespace

const TUid& ContactsReader::ForApplication() const {
  return kContactsUid;
}

void ContactsReader::ReadList(CEikListBox* listbox, AppState* app_state) {
  if (!listbox) return;
  if (!listbox->View()) return;
  MListBoxModel* model = listbox->Model();
  app_state->SetItemCount(model->NumberOfItems());
  if (model->NumberOfItems() > 0) {
    const MDesCArray* array = model->MatchableTextArray();
    app_state->SetSelectedItemIndex(listbox->CurrentItemIndex());
    const TDesC& text = array->MdcaPoint(listbox->CurrentItemIndex());
    app_state->SetSelectedItemText(text);
  }
}

void ContactsReader::Read() {
  app_state_.Reset();
  app_helper_.ReadTitle(&control_tree_, &app_state_);
  // return;
  const TVwsViewId view_id = app_helper_.ReadView(&control_tree_, &app_state_);
  return;
  app_helper_.ReadCba(&control_tree_, &app_state_);
  CEikMenuBar* menu = control_tree_.MenuBar();
  if (menu && menu->IsFocused()) {
    CEikMenuPane* pane = menu->MenuPane();
    app_helper_.ReadMenu(pane, &app_state_);
    if (pane) {
      return;
    }
  }
  // TODO(mikie): move tab reading to AppHelper.
  CAknTabGroup* tabgroup = control_tree_.TabGroup();
  if (tabgroup) {
    app_state_.SetSelectedTabIndex(tabgroup->ActiveTabIndex());
    app_state_.SetTabCount(tabgroup->TabCount());
    const TInt id = tabgroup->ActiveTabId();
    CAknTabGroupAccess* access = (CAknTabGroupAccess*)tabgroup;
    CArrayPtr<CAknTabAccess>* tabs = access->iTabArray;
    for (int i = 0; i < tabs->Count(); ++i) {
      CAknTabAccess* tab = tabs->At(i);
      CCoeControl* tab_control = (CCoeControl*)tab;
      if (tab && tab->iId == id) {
        // Not all tabs have text.
        CEikLabel* label = tab->iLabel;
        if (label) {
          app_state_.SetSelectedTabText(*label->Text());
        }
        break;
      }
    }
  }
  if (view_id.iAppUid != ForApplication()) {
    return;
  }
  const TUint view = view_id.iViewUid.iUid;
  if (view == 1) {
    // Main contacts view
    CCoeControl* top = control_tree_.TopFocusedControl();
    if (top->CountComponentControls() == 0) return;
    CCoeControl* container = top->ComponentControl(0);
    if (container->CountComponentControls() < 2) return;
    CEikListBox* listbox = (CEikListBox*)container->ComponentControl(0);
    CAknSearchField* field = (CAknSearchField*)container->ComponentControl(1);

    ReadList(listbox, &app_state_);

    HBufC* text = HBufC::NewLC(field->TextLength());
    TPtr16 textp = text->Des();
    field->GetSearchText(textp);
    app_state_.SetSearchFieldText(*text);
    CleanupStack::PopAndDestroy(text);
  } else if (view == 4) {
    // Details (read-only) view
    CCoeControl* top = control_tree_.TopFocusedControl();
    if (top->CountComponentControls() == 0) return;
    CCoeControl* container = top->ComponentControl(0);
    if (container->CountComponentControls() < 1) return;
    CEikListBox* listbox = (CEikListBox*)container->ComponentControl(0);

    ReadList(listbox, &app_state_);
  }
}


