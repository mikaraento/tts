// TODO(mikie): this code is still very experimental - it needs to be
// made more performant at the very least, and a more structured reporting
// function generated.

// Something in the control introspection doesn't work right in a GCC-E
// release build - build for debug.

#include "control_walker.h"

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

#include "control_tree.h"
#include "reporting.h"
#include "rtti.h"

// CCoeControl declares an internal class called CCoeRedrawer as a friend.
// We can declare our own class with that name as a convenient way to access
// private parts of CCoeControl.
class CCoeRedrawer {
 public:
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
  if (!listbox->View()) return;
  CTextListBoxModel* model = listbox->Model();
  if (model->NumberOfItems() > 0) {
    TBuf<80> buf = _L("Item ");
    buf.AppendNum(listbox->CurrentItemIndex() + 1);
    buf.Append(_L(" of "));
    buf.AppendNum(model->NumberOfItems());
    buf.Append(_L(": "));
    const TDesC& text = model->ItemText(listbox->CurrentItemIndex());
    buf.Append(text.Left(50));
    logger->Log(buf);
  } else {
    logger->Log(_L("empty"));
  }
}

void Report(CEikListBox* listbox, LoggingState* logger) {
  if (!listbox) return;
  if (!listbox->View()) return;
  MListBoxModel* model = listbox->Model();
  if (model->NumberOfItems() > 0) {
    const MDesCArray* array = model->MatchableTextArray();
    TBuf<80> buf = _L("Item ");
    buf.AppendNum(listbox->CurrentItemIndex() + 1);
    buf.Append(_L(" of "));
    buf.AppendNum(model->NumberOfItems());
    buf.Append(_L(": "));
    const TDesC& text = array->MdcaPoint(listbox->CurrentItemIndex());
    buf.Append(text.Left(50));
    logger->Log(buf);
  } else {
    logger->Log(_L("empty"));
  }
}

void Report(CAknGrid* grid, LoggingState* logger) {
  if (!grid->View()) return;
  CTextListBoxModel* model = grid->Model();
  if (model->NumberOfItems() > 0) {
    TBuf<80> buf = _L("Item ");
    buf.AppendNum(grid->CurrentItemIndex());
    buf.Append(_L(" of "));
    buf.AppendNum(model->NumberOfItems());
    buf.Append(_L(": "));
    const TDesC& text = model->ItemText(grid->CurrentItemIndex());
    buf.Append(text.Left(50));
    logger->Log(buf);
  } else {
    logger->Log(*grid->EmptyGridText());
  }
}

void Report(CEikMenuPane* pane, LoggingState* logger) {
  if (!pane) return;
  if (pane->NumberOfItemsInPane() > 0) {
    TBuf<80> buf = _L("Item ");
    buf.AppendNum(pane->SelectedItem() + 1);
    buf.Append(_L(" of "));
    buf.AppendNum(pane->NumberOfItemsInPane());
    buf.Append(_L(": "));
    const CEikMenuPaneItem::SData& data = pane->ItemDataByIndexL(
        pane->SelectedItem());
    buf.Append(data.iText.Left(50));
    logger->Log(buf);
  } else {
    logger->Log(_L("empty"));
  }
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


// We want to be able detect known control class instances with the minimum
// fuss, so we we write the necessary reporting functions and generate a lookup
// table.
// For the technique http://blog.brush.co.nz/2009/08/xmacros/
struct ClassSig {
  const unsigned char* class_name;
  void* (*GetVTableAddress)(void);
  void (*ReportState)(CCoeControl* control, LoggingState* logger);
};

#if 0
// This code was useful as a starting point but:
//   - it doesn't work on-device
//   - it's expensive
//   - it's good enough to detect CEikListBox rather than the specific children
//   - it's not enough anyway as not all the classes we are interested in
//     can be constructed without arguments
//   - a lot of apps use derived classes so the vtable-pointer-comparison
//     isn't enough.
#define CONTROL(name) \
  void* Get ## name ## VTable() { \
    name * o = new (ELeave) name; \
    void* ptr = *(void**)o; \
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
    & Report ## name ## State },

ClassSig classes[] = {
#include "control_classes.h"
  { 0 },
};
#undef CONTROL
#undef TEXT

#else  // !__WINS__

ClassSig classes[] = {
  { 0 },
};

#endif  // __WINS__

void ReportWithSig(const ClassSig* control_class,
                   CCoeControl* control,
                   LoggingState* logger) {
  TBuf<100> name;
  name.Copy(TPtrC8(control_class->class_name));
  logger->Log(name);
  (*(control_class->ReportState))(control, logger);
}

void InspectControl(CCoeControl* control, LoggingState* logger, int depth) {
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
  SafeTypes safe;
  UnsafeTypes unsafe;
  CEikTextListBox* list = unsafe.IsEikTextListBox(control);
  if (list) {
    logger->Log(_L("CEikTextListBox"));
    Report(list, logger);
    return;
  }
  // CAknGrid derives from CEikListBox, let's check for it first.
  CAknGrid* grid = unsafe.IsAknGrid(control);
  if (grid) {
    logger->Log(_L("CAknGrid"));
    Report(grid, logger);
    return;
  }
  CEikListBox* list2 = unsafe.IsEikListBox(control);
  if (list2) {
    logger->Log(_L("CEikListBox"));
    Report(list2, logger);
    return;
  }
  /*
  CAknSelectionListDialog* dialog = IsAknSelectionListDialog(control);
  if (dialog) {
    logger->Log(_L("CAknSelectionListDialog"));
    Report(dialog, logger);
    return;
  }*/
  CAknSearchField* searchfield = safe.IsAknSearchField(control);
  if (searchfield) {
    logger->Log(_L("CAknSearchField"));
    Report(searchfield, logger);
    return;
  }
  CEikEdwin* edwin = safe.IsEikEdwin(control);
  if (edwin) {
    logger->Log(_L("CEikEdwin"));
    Report(edwin, logger);
    return;
  }
  CEikLabel* label = safe.IsEikLabel(control);
  if (label) {
    logger->Log(_L("CEikLabel"));
    Report(label, logger);
    return;
  }
  CEikMenuPane* menubar = safe.IsEikMenuPane(control);
  if (menubar) {
    logger->Log(_L("CEikMenuPane"));
    Report(menubar, logger);
    return;
  }
#if 0
  // HACKHACK. This code was used to verify that a certain control indeed
  // is the grid in the app menu.
  if (depth == 2 &&
      control->Size().iWidth == 238 &&
      control->Size().iHeight == 264) {
    logger->Log(_L("CAknGrid"));
    Report((CAknGrid*)control, logger);
    return;
  }
#endif

  // I also experimented with matching method pointers and vtables but was
  // not able to weed out the false positives.

  logger->Log(_L("unknown"));
}

void WalkOne(CCoeControl* control, LoggingState* logger, int depth) {
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
  InspectControl(control, logger, depth);
  {
    // These variables make it easy to check whether an unknown control
    // looks like a known type under interactive debugging.
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
    WalkOne(control->ComponentControl(i), logger, depth + 1);
  }
  logger->DecreaseIndent();
}

void WalkWindows(TUint32 handle, LoggingState* logger, TBool forward) {
  while (handle) {
    // With CEikonEnv the RWindow handle must always be a pointer to
    // a CCoeControl (see http://mikie.iki.fi/wordpress/?p=20).
    CCoeControl* control = (CCoeControl*)handle;
    WalkOne(control, logger, 0);
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
  }
}

void WalkArray(const ControlTree::ControlArray& array, LoggingState* logger) {
  for (int i = 0; i < array.Count(); ++i) {
    WalkOne(array[i], logger, 0);
  }
}

}  // namespace

// From ER5 sources. On SOS 9 the length of an element is 16 instead of 12
// so I've added an iUnknown field.
struct SStackedControl
    {
    CCoeControl* iControl;
    TInt iPriority;
    TInt iFlags;
    TInt iUnknown;
    };

class CCoeControlStack : public CArrayFixFlat<SStackedControl>
    {
public:
    inline CCoeControlStack() : CArrayFixFlat<SStackedControl>(2) { } // granularity of two
    TKeyResponse OfferKeyL(const TKeyEvent& aKeyEvent,TEventCode aType);
public:
    TInt iKeyIndex;
    };

namespace {
void WalkStack(LoggingState* logger) {
  CCoeAppUiAccess* appui = (CCoeAppUiAccess*)CEikonEnv::Static()->AppUi();
  CCoeControlStack* stack = appui->iStack;
  logger->Log(_L("From stack"));
  for (int i = 0; i < stack->Count(); ++i) {
    SStackedControl s = stack->At(i);
    CCoeControl* control = s.iControl;
    WalkOne(control, logger, 0);
  }
}
}  // namespace

ControlWalker::ControlWalker() {}

ControlWalker::~ControlWalker() {}

void ControlWalker::Walk(LoggingState* logger) {
  CEikonEnv* env = CEikonEnv::Static();

  TBuf<100> header;
  ControlTree control_tree;
  control_tree.RefreshControlStack();
  control_tree.RefreshWindowList();
  CAknTitlePane* title_pane = control_tree.TitlePane();
  if (title_pane) {
    header.Append(title_pane->Text()->Left(50));
  }
  const TUint32 fepsetup_uid = 0xe0003473;
  const TUint32 menu_uid = 0x101f4cd2;

  RProcess me;
  me.Open(me.Id());
  const TUint32 uid = me.SecureId().iId;
#if 0
  // You can easily log just one app by switching on uid here.
  if (uid != menu_uid) {
    me.Close();
    return;
  }
#endif
  header.Append(_L(" uid: "));
  header.AppendNum(uid, EHex);
  header.Append(_L(" process_id: "));
  header.AppendNum(me.Id());
  me.Close();
  CAknAppUi* appui = (CAknAppUi*)env->AppUi();
  TVwsViewId view_id;
  TInt view_found = appui->GetActiveViewId(view_id);
  CCoeControl* cba = control_tree.Cba();
  if (view_found != KErrNotFound) {
    header.Append(_L(" view: "));
    header.AppendNum(view_id.iAppUid.iUid, EHex);
    header.Append(_L(" "));
    header.AppendNum(view_id.iViewUid.iUid, EHex);
    if (!cba) {
      CAknViewAppUi* viewappui = (CAknViewAppUi*)appui;
      CAknView* view = viewappui->View(view_id.iViewUid);
      if (view) cba = view->iCba;
    }
  }
  logger->Log(header);
  logger->IncreaseIndent();

  CCoeControl* top = control_tree.TopFocusedControl();
  if (1) {
    // TopFocusedControl() returns NULL in the menu on-device, so it's
    // not good enough.
    logger->Log(_L("From TopFocusedControl"));
    WalkOne(top, logger, 0);
  }

  if (0) {
    WalkStack(logger);
  } else {
    logger->Log(_L("From ControlStack"));
    WalkArray(control_tree.ControlStack(), logger);
  }

  if (1) {
    logger->Log(_L("From Windows"));
    WalkArray(control_tree.WindowList(), logger);
  }

  if (0) {
    // This walks controls by starting from the first child window of the
    // application window group. However, the handle of the first child
    // points to a non-window-owning 0-size control?
    logger->Log(_L("From RootWin()"));
    WalkWindows(env->RootWin().Child(), logger, ETrue);
  }

  if (0) {
    // This walks controls by windows by starting from the topmost
    // and following sibling windows. This gets to tabs, the CBA etc. but
    // we aren't really interested in them. It could be helpful for apps
    // that create several window-owning controls for main display.
    logger->Log(_L("From topmost window"));
    WalkWindows((TUint32)top, logger, ETrue);
  }

  logger->DecreaseIndent();
}
