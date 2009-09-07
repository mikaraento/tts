#include "app_helper.h"

#include <aknappui.h>
#include <akntitle.h>
#include <eikbtgpc.h>
#include <eiklabel.h>
#include <eikmenup.h>

#include "app_reader.h"
#include "control_tree.h"

// Although CEikMenuPane declares an IMPORT_C CascadeMenuPane() function
// it's not actually in the LIBs.
class CEikMenuPaneExtension {
 public:
  static inline CEikMenuPane* CascadeMenuPane(CEikMenuPane* pane) {
    return pane->iCascadeMenuPane;
  }
};

void AppHelper::ReadMenu(CEikMenuPane* pane, AppState* app_state) {
  if (!pane) return;
  app_state->SetIsShowingMenuOrPopup(ETrue);
  // After that, let's walk down to the most cascaded menu. This will
  // be the active one as cascaded menus are automatically removed
  // when they leave focus.
  CEikMenuPane* cascade = pane;
  while (cascade) {
    pane = cascade;
    cascade = CEikMenuPaneExtension::CascadeMenuPane(pane);
  }
  app_state->SetItemCount(pane->NumberOfItemsInPane());
  if (pane->NumberOfItemsInPane() > 0) {
    // A menu might not have any items even if it's the normal convention.
    app_state->SetSelectedItemIndex(pane->SelectedItem());
    const CEikMenuPaneItem::SData& data = pane->ItemDataByIndexL(
        pane->SelectedItem());
    app_state->SetSelectedItemText(data.iText);
  }
}

void AppHelper::ReadTitle(ControlTree* control_tree, AppState* app_state) {
  CAknTitlePane* title = control_tree->TitlePane();
  if (title) app_state->SetTitle(*title->Text());
}

TVwsViewId AppHelper::ReadView(ControlTree* control_tree, AppState* app_state) {
  TVwsViewId view_id;
  TInt view_found = control_tree->AppUi()->GetActiveViewId(view_id);
  if (view_found != KErrNotFound) {
    app_state->SetAppUid(view_id.iAppUid);
    app_state->SetViewUid(view_id.iViewUid);
  }
  return view_id;
}

CEikButtonGroupContainer* AppHelper::ReadCba(ControlTree* control_tree,
                                             AppState* app_state) {
  // From
  // http://wiki.forum.nokia.com/index.php/TSS000675_-_Retrieving_text_for_softkey_labels
  CEikButtonGroupContainer* cba = control_tree->Cba();
  TInt first_command;
  TInt second_command;
  if (cba) {
    MEikButtonGroup* buttonGroup = cba->ButtonGroup();
    for (int pos = 0; pos < 3; ++pos) {
      const TInt cmd_id = buttonGroup->CommandId(pos);
      CCoeControl* button = buttonGroup->GroupControlById(cmd_id);
      if (button && buttonGroup->IsCommandVisible(cmd_id)) {
        CEikLabel* label =
                static_cast<CEikLabel*>(button->ComponentControl(0));
        const TDesC* txt = label->Text();
        if (pos == 0) {
          app_state->SetFirstSoftkey(*txt);
          first_command = cmd_id;
        } else {
          app_state->SetSecondSoftkey(*txt);
          second_command = cmd_id;
        }
      }
    }
  }
  return cba;
}
