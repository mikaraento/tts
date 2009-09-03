#include "rtti.h"

#include "modifed_system_include/akngrid.h"
#include "modifed_system_include/aknsfld.h"
#include "modifed_system_include/aknview.h"
#include "modifed_system_include/coeaui.h"
#include "modifed_system_include/eiklbx.h"

#include <aknform.h>
#include <aknlistquerycontrol.h>
#include <aknlists.h>
#include <aknquerydialog.h>
#include <aknselectionlist.h>
#include <aknsfld.h>
#include <aknsutils.h>
#include <akntitle.h>
#include <aknviewappui.h>
#include <eikappui.h>
#include <eikdpage.h>
#include <eikedwin.h>
#include <eikenv.h>
#include <eiklabel.h>
#include <eikspane.h>
#include <frmtview.h>

bool IsValidPointer(void* p) {
  return (TUint)p % 4 == 0 &&
         p > User::Heap().Base() &&
         p < User::Heap().Base() + User::Heap().Size();
}

// A heuristic check for CEikTextListBox-derived classes: check if they
// seem to have a reasonable CTextListBoxModel Model().
CEikTextListBox* UnsafeTypes::IsEikTextListBox(CCoeControl* control) {
  CEikTextListBox* list = (CEikTextListBox*)control;
  CTextListBoxModel* model = list->Model();
  CEikTextListBox* ret = NULL;
  void* model_p = (void*)model;
  if (IsValidPointer(model_p)) {
    CTextListBoxModel* created = new (ELeave) CTextListBoxModel;
    void* created_p = (void*)created;
    // And point to a CTextListBoxModel vtable.
    if (*(void**)model_p == *(void**)created_p) {
      ret = list;
    }
    delete created;
  }
  return ret;
}

#ifndef __WINS__
typedef RHeap::SCell Cell;
#else
typedef RHeap::SDebugCell Cell;
#endif


// A heuristic check for CEikListBox-derived classes: check if they
// seem to have reasonable internals.
CEikListBox* UnsafeTypes::IsEikListBox(CCoeControl* control) {
  CEikListBoxAccess* list = (CEikListBoxAccess*)control;
  MListBoxModel* model = list->iModel;
  CListBoxView* view = list->iView;
  CEikListBox* ret = NULL;
  void* model_p = (void*)model;
  Cell* allocation = (Cell*)((char*)control - sizeof(Cell));
  if (allocation->len < sizeof(CEikListBox)) return NULL;
  // The pointer should be aligned and within the heap.
  if (IsValidPointer(model) && IsValidPointer(view)) {
    CEikListBoxAccess* access = (CEikListBoxAccess*)control;
    const int item_height = access->iItemHeight;
    const int sb_frame_owned = access->iSBFrameOwned;
    const int adj = access->iViewRectHeightAdjustment;
    const int required_height = access->iRequiredHeightInNumOfItems;
    if (item_height >= 0 && item_height < 500 &&
        (sb_frame_owned == 0 || sb_frame_owned == 1) &&
        adj > -500 && adj < 500 &&
        required_height >= 0 && required_height < 100) {
      ret = (CEikListBox*)control;
    }
  }
  return ret;
}

CAknGrid* UnsafeTypes::IsAknGrid(CCoeControl* control) {
  if (!IsEikListBox(control)) return NULL;
  CAknGridAccess* access = (CAknGridAccess*)control;
  const int min_width = access->iMinColWidth;
  const int current_is_valid = access->iCurrentIsValid;
  const int cols = access->iNumOfColsInView;
  const int rows = access->iNumOfRowsInView;
  if (min_width >= 0 && min_width < 500 &&
      (current_is_valid == (int)EFalse || current_is_valid == (int)ETrue) &&
      cols > 0 && cols < 50 &&
      rows > 0 && rows < 50) {
    return (CAknGrid*)control;
  }
  return NULL;
}

/*
CAknSelectionListDialog* IsAknSelectionListDialog(CCoeControl* control) {
  CDesC16ArrayFlat* array = new (ELeave) CDesC16ArrayFlat(1);
  AccessSelectionListDialog* created =
      new (ELeave) AccessSelectionListDialog(array);
  if (*(void**)created == *(void**)control) {
    delete created;
    delete array;
    return (CAknSelectionListDialog*)control;
  }
  delete created;
  delete array;
  return NULL;
}
*/

namespace {
class EmptyControl : public CCoeControl {
 public:
  void ConstructL() { CreateWindowL(); }
};
}  // namespace

CAknSearchField* SafeTypes::IsAknSearchField(CCoeControl* control) {
  EmptyControl* parent = new (ELeave) EmptyControl;
  parent->ConstructL();
  CAknSearchField* created = CAknSearchField::NewL(
      *parent, CAknSearchField::ESearch, NULL, 10);
  if (*(void**)created == *(void**)control) {
    delete created;
    delete parent;
    return (CAknSearchField*)control;
  }
  delete created;
  delete parent;
  return NULL;
}

CEikEdwin* SafeTypes::IsEikEdwin(CCoeControl* control) {
  CEikEdwin* created = new CEikEdwin;
  if (*(void**)created == *(void**)control) {
    delete created;
    return (CEikEdwin*)control;
  }
  delete created;
  return NULL;
}

CEikLabel* SafeTypes::IsEikLabel(CCoeControl* control) {
  CEikLabel* created = new CEikLabel;
  if (*(void**)created == *(void**)control) {
    delete created;
    return (CEikLabel*)control;
  }
  delete created;
  return NULL;
}

namespace {
class EikMenuObserver : public MEikMenuObserver {
  virtual void SetEmphasis(CCoeControl* aMenuControl,TBool aEmphasis) {}
  virtual void ProcessCommandL(TInt aCommandId) {}
};
}  // namespace

CEikMenuPane* SafeTypes::IsEikMenuPane(CCoeControl* control) {
  EikMenuObserver obs;
  CEikMenuPane* created = new CEikMenuPane(&obs);
  if (*(void**)created == *(void**)control) {
    delete created;
    return (CEikMenuPane*)control;
  }
  delete created;
  return NULL;
}
