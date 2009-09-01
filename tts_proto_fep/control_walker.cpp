#include "control_walker.h"

#include <aknform.h> 
#include <aknlistquerycontrol.h>
#include <aknlists.h> 
#include <aknquerydialog.h>
#include <aknselectionlist.h>
#include <aknsfld.h>
#include <aknsutils.h>
#include <akntitle.h>
#include <eikappui.h>
#include <eikdpage.h>
#include <eikedwin.h>
#include <eikenv.h>
#include <eiklabel.h>
#include <eikspane.h>
#include <frmtview.h> 

#include "modifed_system_include/akngrid.h"
#include "modifed_system_include/aknsfld.h"
#include "modifed_system_include/eiklbx.h"
#include "reporting.h"

namespace {
typedef void (CCoeControl::*DrawFunc)(const TRect& aRect) const;
}  // namespace

// CCoeControl declares an internal class called CCoeRedrawer as a friend.
// We can declare our own class with that name as a convenient way to access
// private parts of CCoeControl.
class CCoeRedrawer {
 public:
  inline static DrawFunc GetDraw(CCoeControl* ctrl) { return &ctrl->Draw; }
  inline static RWindow* Window(CCoeControl* ctrl) { return &ctrl->Window(); }
};

namespace {

void Report(CEikLabel* label, LoggingState* logger) {
  if (label->Text()) {
    logger->Log(*label->Text());
  } else {
    logger->Log(_L("[empty]"));
  }
}

void Report(CEikEdwin* edwin, LoggingState* logger) {
  HBufC* text = edwin->GetTextInHBufL();
  CleanupStack::PushL(text);
  logger->Log(*text);
  CleanupStack::PopAndDestroy(text);
}

void Report(CEikTextListBox* listbox, LoggingState* logger) {
  CTextListBoxModel* model = listbox->Model();
  TBuf<80> buf = _L("Item ");
  buf.AppendNum(listbox->CurrentItemIndex() + 1);
  buf.Append(_L(" of "));
  buf.AppendNum(model->NumberOfItems());
  buf.Append(_L(": "));
  const TDesC& text = model->ItemText(listbox->CurrentItemIndex());
  buf.Append(text.Left(50));
  logger->Log(buf);
}

void Report(CEikListBox* listbox, LoggingState* logger) {
  if (!listbox) return;
  MListBoxModel* model = listbox->Model();
  const MDesCArray* array = model->MatchableTextArray();
  TBuf<80> buf = _L("Item ");
  buf.AppendNum(listbox->CurrentItemIndex() + 1);
  buf.Append(_L(" of "));
  buf.AppendNum(model->NumberOfItems());
  buf.Append(_L(": "));
  const TDesC& text = array->MdcaPoint(listbox->CurrentItemIndex());
  buf.Append(text.Left(50));
  logger->Log(buf);
}

void Report(CAknGrid* grid, LoggingState* logger) {
  CTextListBoxModel* model = grid->Model();
  TBuf<80> buf = _L("Item ");
  buf.AppendNum(grid->CurrentItemIndex());
  buf.Append(_L(" of "));
  buf.AppendNum(model->NumberOfItems());
  buf.Append(_L(": "));
  const TDesC& text = model->ItemText(grid->CurrentItemIndex());
  buf.Append(text.Left(50));
  logger->Log(buf);
}

void Report(CAknSearchField* field, LoggingState* logger) {
  HBufC* text = HBufC::NewLC(field->TextLength());
  TPtr16 textp = text->Des();
  field->GetSearchText(textp);
  logger->Log(*text);
  CleanupStack::PopAndDestroy(text);
  CAknSearchFieldAccess* access = (CAknSearchFieldAccess*)field;
  Report(access->iListBox, logger);
}

void Report(CAknDialog* dialog, LoggingState* logger) {
}

void Report(CAknForm* dialog, LoggingState* logger) {
}

class AccessSelectionListDialog : public CAknSelectionListDialog {
 public:
  AccessSelectionListDialog(CDesC16Array* array)
      : CAknSelectionListDialog(item, array, NULL) {
  }
  CEikListBox* list() { return ListBox(); }
 private:
  TInt item;
};

void Report(CAknSelectionListDialog* dialog, LoggingState* logger) {
  AccessSelectionListDialog* access = (AccessSelectionListDialog*)dialog;
  Report(access->list(), logger);
}

// A heuristic check for CEikTextListBox-derived classes: check if they
// seem to have a reasonable CTextListBoxModel Model().
CEikTextListBox* IsEikTextListBox(CCoeControl* control) {
  CEikTextListBox* list = (CEikTextListBox*)control;
  CTextListBoxModel* model = list->Model();
  CEikTextListBox* ret = NULL;
  void* model_p = (void*)model;
  // The pointer should be aligned and within the heap.
  if ( (TUint)model_p % 4 == 0 &&
      model_p > User::Heap().Base() &&
      model_p < User::Heap().Base() + User::Heap().Size()) {
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
CEikListBox* IsEikListBox(CCoeControl* control) {
  CEikListBox* list = (CEikListBox*)control;
  MListBoxModel* model = list->Model();
  CEikListBox* ret = NULL;
  void* model_p = (void*)model;
  Cell* allocation = (Cell*)((char*)control - sizeof(Cell));
  if (allocation->len < sizeof(CEikListBox)) return NULL;
  // The pointer should be aligned and within the heap.
  if ( (TUint)model_p % 4 == 0 &&
      model_p > User::Heap().Base() &&
      model_p < User::Heap().Base() + User::Heap().Size()) {
    CEikListBoxAccess* access = (CEikListBoxAccess*)control;
    const int item_height = access->iItemHeight;
    const int sb_frame_owned = access->iSBFrameOwned;
    const int adj = access->iViewRectHeightAdjustment;
    const int required_height = access->iRequiredHeightInNumOfItems;
    if (item_height >= 0 && item_height < 500 &&
        (sb_frame_owned == 0 || sb_frame_owned == 1) &&
        adj > -500 && adj < 500 &&
        required_height >= 0 && required_height < 100) {
      ret = list;
    }
  }
  return ret;
}

CAknGrid* IsAknGrid(CCoeControl* control) {
  if (!IsEikListBox(control)) return NULL;
  CAknGridAccess* access = (CAknGridAccess*)control;
  const int min_width = access->iMinColWidth;
  const int current_is_valid = access->iCurrentIsValid;
  const int cols = access->iNumOfColsInView;
  const int rows = access->iNumOfRowsInView;
  if (min_width >= 0 && min_width < 500 &&
      (current_is_valid == (int)EFalse || current_is_valid == (int)ETrue) &&
      cols >= 0 && cols < 500 &&
      rows >= 0 && rows < 500) {
    return (CAknGrid*)control;
  }
  return NULL;
}

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

class EmptyControl : public CCoeControl {
 public:
  void ConstructL() { CreateWindowL(); }
};

CAknSearchField* IsAknSearchField(CCoeControl* control) {
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

// We want to be able detect known control class instances with the minimum
// fuss, so we we write the necessary reporting functions and generate a lookup
// table.
// For the technique http://blog.brush.co.nz/2009/08/xmacros/
struct ClassSig {
  const unsigned char* class_name;
  void* (*GetVTableAddress)(void);
  DrawFunc (*GetDrawAddress)(void);
  void (*ReportState)(CCoeControl* control, LoggingState* logger);
};

#define CONTROL(name) \
  void* Get ## name ## VTable() { \
    name * o = new (ELeave) name; \
    void* ptr = *(void**)o; \
    delete o; \
    return ptr; \
  } \
  DrawFunc Get ## name ## Draw() { \
    CCoeControl * o = new (ELeave) name; \
    DrawFunc ptr = CCoeRedrawer::GetDraw(o); \
    delete o; \
    return ptr; \
  } \
  void Report ## name ## State(CCoeControl* control, LoggingState* logger) { \
    name * o = reinterpret_cast< name * >(control); \
    Report(o, logger); \
  }
#include "control_classes.h"
#undef CONTROL

#define TEXT(name) (const unsigned char*)#name
#define CONTROL(name) \
  { TEXT(name), \
    & Get ## name ## VTable, \
    & Get ## name ## Draw, \
    & Report ## name ## State },

ClassSig classes[] = {
#include "control_classes.h"
  { 0 },
};
#undef CONTROL
#undef TEXT

void ReportWithSig(const ClassSig* control_class,
                   CCoeControl* control,
                   LoggingState* logger) {
  TBuf<100> name;
  name.Copy(TPtrC8(control_class->class_name));
  logger->Log(name);
  (*(control_class->ReportState))(control, logger);  
}

void InspectControl(CCoeControl* control, LoggingState* logger) {
  void* vtable = *(void**)control;
  for (const ClassSig* control_class = classes;
       control_class->class_name;
       ++control_class) {
    const bool vtable_match = vtable == (*(control_class->GetVTableAddress))();
    if (vtable_match) {
      ReportWithSig(control_class, control, logger);
      return;
    }
  }
  CEikTextListBox* list = IsEikTextListBox(control);
  if (list) {
    logger->Log(_L("CEikTextListBox"));
    Report(list, logger);
    return;
  }
  // CAknGrid derives from CEikListBox, let's check for it first.
  CAknGrid* grid = IsAknGrid(control);
  if (grid) {
    logger->Log(_L("CAknGrid"));
    Report(grid, logger);
    return;    
  }
  CEikListBox* list2 = IsEikListBox(control);
  if (list2) {
    logger->Log(_L("CEikListBox"));
    Report(list2, logger);
    return;
  }
  CAknSelectionListDialog* dialog = IsAknSelectionListDialog(control);
  if (dialog) {
    logger->Log(_L("CAknSelectionListDialog"));
    Report(dialog, logger);
    return;    
  }
  CAknSearchField* searchfield = IsAknSearchField(control);
  if (searchfield) {
    logger->Log(_L("CAknSearchField"));
    Report(searchfield, logger);
    return;    
  }

  // I also experimented with matching method pointers and vtables but was
  // not able to weed out the false positives.

  logger->Log(_L("unknown"));
}

void WalkOne(CCoeControl* control, LoggingState* logger) {
  if (!control) return;
  const int child_count = control->CountComponentControls();
  TBuf<100> buf = _L("pos ");
  buf.AppendNum(control->Position().iX);
  buf.Append(' ');
  buf.AppendNum(control->Position().iY);
  buf.Append(' ');
  buf.AppendNum(control->Size().iWidth);
  buf.Append(' ');
  buf.AppendNum(control->Size().iHeight);
  buf.Append(_L(" children "));
  buf.AppendNum(child_count);
  logger->Log(buf);
  InspectControl(control, logger);
  {
    CEikListBox* list = (CEikListBox*)control;
    CAknDialog* dialog = (CAknDialog*)control;
    CTextView* textv = (CTextView*)control;
    CEikBorderedControl* bordered = (CEikBorderedControl*)control;
    CAknGrid* grid = (CAknGrid*)control;
    CAknListQueryControl* query = (CAknListQueryControl*)control;
    TInt x;
    x = 0;
  }
  if (child_count == 0) return;
  logger->IncreaseIndent();
  for (int i = 0; i < child_count; ++i) {
    WalkOne(control->ComponentControl(i), logger);
  }
  logger->DecreaseIndent();
}

void WalkWindows(TUint32 handle, LoggingState* logger) {
  while (handle) {
    // With CEikonEnv the RWindow handle must always be a pointer to
    // a CCoeControl (see http://mikie.iki.fi/wordpress/?p=20).
    CCoeControl* control = (CCoeControl*)handle;
    WalkOne(control, logger);
    RWindow* window = CCoeRedrawer::Window(control);
    if (!window) {
      handle = 0;
    } else {
      handle = window->NextSibling();
    }
  }
}
}  // namespace

ControlWalker::ControlWalker() : CActive(CActive::EPriorityLow) {
  CActiveScheduler::Add(this);
}

ControlWalker::~ControlWalker() {
  Cancel();
}

void ControlWalker::TriggerWalk(LoggingState* logger) {
  if (IsActive()) return;
  logger_ = logger;
  TRequestStatus* s = &iStatus;
  User::RequestComplete(s, KErrNone);
  SetActive();
}

void ControlWalker::DoCancel() { }

void ControlWalker::RunL() {
  Walk(logger_);
}

void ControlWalker::Walk(LoggingState* logger) {
  CEikonEnv* env = CEikonEnv::Static();
  
  TBuf<100> header;
  CEikStatusPane* sp = CEikonEnv::Static()->AppUiFactory()->StatusPane();
  CAknTitlePane* titlePane = NULL;
  if (sp) {
    titlePane = (CAknTitlePane*)
        sp->ControlL(TUid::Uid(EEikStatusPaneUidTitle));
  }
  if (titlePane) {
    header.Append(titlePane->Text()->Left(50));
  }
  header.Append(_L(" uid: "));
  RProcess me;
  me.Open(me.Id());
  TUint32 uid = me.SecureId().iId;
  header.AppendNum(uid, EHex);
  me.Close();
  CAknAppUi* appui = (CAknAppUi*)env->AppUi();
  TVwsViewId view_id;
  TInt view_found = appui->GetActiveViewId(view_id);
  if (view_found != KErrNotFound) {
    header.Append(_L(" view: "));
    header.AppendNum(view_id.iAppUid.iUid, EHex);
    header.Append(_L(" "));
    header.AppendNum(view_id.iViewUid.iUid, EHex);
  }
  logger->Log(header);
  
  CCoeControl* top = appui->TopFocusedControl();
  logger->Log(_L("From TopFocusedControl"));
  WalkOne(top, logger);

  if (0) {
    // This walks controls by starting from the first child window of the
    // application window group. However, the handle of the first child
    // points to a non-window-owning 0-size control?
    logger->Log(_L("From RootWin()"));
    WalkWindows(env->RootWin().Child(), logger);
  }

  if (0) {
    // This walks controls by windows by starting from the topmost
    // and following sibling windows. This gets to tabs, the CBA etc. but
    // we aren't really interested in them. It could be helpful for apps
    // that create several window-owning controls for main display.
    logger->Log(_L("From topmost window"));
    WalkWindows((TUint32)top, logger);
  }
}
