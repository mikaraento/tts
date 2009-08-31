# 1 "control_walker.cpp"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "control_walker.cpp"
# 1 "control_walker.h" 1





class LoggingState;





class ControlWalker : public CActive{
 public:
  ControlWalker();
  ~ControlWalker();
  void TriggerWalk(LoggingState* logger);
 private:
  void RunL();
  void DoCancel();
  void Walk(LoggingState* logger);
  LoggingState* logger_;
};
# 2 "control_walker.cpp" 2







# 1 "reporting.h" 1
# 10 "reporting.h"
class LoggingState {
 public:
  static LoggingState* Get();
  static void SetupTls();
  static void TeardownTls();
  LoggingState();
  ~LoggingState();
  void IncreaseIndent();
  void DecreaseIndent();
  void Log(const TDesC& message);
 private:
  TBuf<20> indent_;
  TBuf<50> process_name_;
  TBuf<128> buffer_;
  RFileLogger logger_;
};
# 10 "control_walker.cpp" 2

namespace {






void Report(CEikLabel* label, LoggingState* logger) {
  logger->Log(*label->Text());
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
  buf.AppendNum(listbox->CurrentItemIndex());
  buf.Append(_L(" of "));
  buf.AppendNum(model->NumberOfItems());
  buf.Append(_L(": "));
  const TDesC& text = model->ItemText(listbox->CurrentItemIndex());
  buf.Append(text.Left(50));
  logger->Log(text);
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
  logger->Log(text);
}

typedef void (CCoeControl::*DrawFunc)(const TRect& aRect) const;

struct ClassSig {
  const unsigned char* class_name;
  void* (*GetVTableAddress)(void);
  DrawFunc (*GetDrawAddress)(void);
  void (*ReportState)(CCoeControl* control, LoggingState* logger);
};
# 83 "control_walker.cpp"
# 1 "control_classes.h" 1




void* GetCEikLabelVTable() { CEikLabel * o = new (ELeave) CEikLabel; void* ptr = *(void**)o; delete o; return ptr; } class AccessCEikLabel : public CEikLabel { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCEikLabelDraw() { AccessCEikLabel * o = new (ELeave) AccessCEikLabel; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCEikLabelState(CCoeControl* control, LoggingState* logger) { CEikLabel * o = reinterpret_cast< CEikLabel * >(control); Report(o, logger); }
void* GetCAknColumnListBoxVTable() { CAknColumnListBox * o = new (ELeave) CAknColumnListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknColumnListBox : public CAknColumnListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknColumnListBoxDraw() { AccessCAknColumnListBox * o = new (ELeave) AccessCAknColumnListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknColumnListBoxState(CCoeControl* control, LoggingState* logger) { CAknColumnListBox * o = reinterpret_cast< CAknColumnListBox * >(control); Report(o, logger); }
void* GetCAknSingleStyleListBoxVTable() { CAknSingleStyleListBox * o = new (ELeave) CAknSingleStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknSingleStyleListBox : public CAknSingleStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknSingleStyleListBoxDraw() { AccessCAknSingleStyleListBox * o = new (ELeave) AccessCAknSingleStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknSingleStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknSingleStyleListBox * o = reinterpret_cast< CAknSingleStyleListBox * >(control); Report(o, logger); }
void* GetCAknSingleNumberStyleListBoxVTable() { CAknSingleNumberStyleListBox * o = new (ELeave) CAknSingleNumberStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknSingleNumberStyleListBox : public CAknSingleNumberStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknSingleNumberStyleListBoxDraw() { AccessCAknSingleNumberStyleListBox * o = new (ELeave) AccessCAknSingleNumberStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknSingleNumberStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknSingleNumberStyleListBox * o = reinterpret_cast< CAknSingleNumberStyleListBox * >(control); Report(o, logger); }
void* GetCAknSingleHeadingStyleListBoxVTable() { CAknSingleHeadingStyleListBox * o = new (ELeave) CAknSingleHeadingStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknSingleHeadingStyleListBox : public CAknSingleHeadingStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknSingleHeadingStyleListBoxDraw() { AccessCAknSingleHeadingStyleListBox * o = new (ELeave) AccessCAknSingleHeadingStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknSingleHeadingStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknSingleHeadingStyleListBox * o = reinterpret_cast< CAknSingleHeadingStyleListBox * >(control); Report(o, logger); }
void* GetCAknSingleGraphicStyleListBoxVTable() { CAknSingleGraphicStyleListBox * o = new (ELeave) CAknSingleGraphicStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknSingleGraphicStyleListBox : public CAknSingleGraphicStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknSingleGraphicStyleListBoxDraw() { AccessCAknSingleGraphicStyleListBox * o = new (ELeave) AccessCAknSingleGraphicStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknSingleGraphicStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknSingleGraphicStyleListBox * o = reinterpret_cast< CAknSingleGraphicStyleListBox * >(control); Report(o, logger); }
void* GetCAknSingleGraphicHeadingStyleListBoxVTable() { CAknSingleGraphicHeadingStyleListBox * o = new (ELeave) CAknSingleGraphicHeadingStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknSingleGraphicHeadingStyleListBox : public CAknSingleGraphicHeadingStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknSingleGraphicHeadingStyleListBoxDraw() { AccessCAknSingleGraphicHeadingStyleListBox * o = new (ELeave) AccessCAknSingleGraphicHeadingStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknSingleGraphicHeadingStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknSingleGraphicHeadingStyleListBox * o = reinterpret_cast< CAknSingleGraphicHeadingStyleListBox * >(control); Report(o, logger); }
void* GetCAknSingleNumberHeadingStyleListBoxVTable() { CAknSingleNumberHeadingStyleListBox * o = new (ELeave) CAknSingleNumberHeadingStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknSingleNumberHeadingStyleListBox : public CAknSingleNumberHeadingStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknSingleNumberHeadingStyleListBoxDraw() { AccessCAknSingleNumberHeadingStyleListBox * o = new (ELeave) AccessCAknSingleNumberHeadingStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknSingleNumberHeadingStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknSingleNumberHeadingStyleListBox * o = reinterpret_cast< CAknSingleNumberHeadingStyleListBox * >(control); Report(o, logger); }
void* GetCAknSingleLargeStyleListBoxVTable() { CAknSingleLargeStyleListBox * o = new (ELeave) CAknSingleLargeStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknSingleLargeStyleListBox : public CAknSingleLargeStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknSingleLargeStyleListBoxDraw() { AccessCAknSingleLargeStyleListBox * o = new (ELeave) AccessCAknSingleLargeStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknSingleLargeStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknSingleLargeStyleListBox * o = reinterpret_cast< CAknSingleLargeStyleListBox * >(control); Report(o, logger); }
void* GetCAknDoubleStyleListBoxVTable() { CAknDoubleStyleListBox * o = new (ELeave) CAknDoubleStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknDoubleStyleListBox : public CAknDoubleStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknDoubleStyleListBoxDraw() { AccessCAknDoubleStyleListBox * o = new (ELeave) AccessCAknDoubleStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknDoubleStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknDoubleStyleListBox * o = reinterpret_cast< CAknDoubleStyleListBox * >(control); Report(o, logger); }
void* GetCAknDoubleStyle2ListBoxVTable() { CAknDoubleStyle2ListBox * o = new (ELeave) CAknDoubleStyle2ListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknDoubleStyle2ListBox : public CAknDoubleStyle2ListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknDoubleStyle2ListBoxDraw() { AccessCAknDoubleStyle2ListBox * o = new (ELeave) AccessCAknDoubleStyle2ListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknDoubleStyle2ListBoxState(CCoeControl* control, LoggingState* logger) { CAknDoubleStyle2ListBox * o = reinterpret_cast< CAknDoubleStyle2ListBox * >(control); Report(o, logger); }
void* GetCAknDoubleNumberStyleListBoxVTable() { CAknDoubleNumberStyleListBox * o = new (ELeave) CAknDoubleNumberStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknDoubleNumberStyleListBox : public CAknDoubleNumberStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknDoubleNumberStyleListBoxDraw() { AccessCAknDoubleNumberStyleListBox * o = new (ELeave) AccessCAknDoubleNumberStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknDoubleNumberStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknDoubleNumberStyleListBox * o = reinterpret_cast< CAknDoubleNumberStyleListBox * >(control); Report(o, logger); }
void* GetCAknDoubleTimeStyleListBoxVTable() { CAknDoubleTimeStyleListBox * o = new (ELeave) CAknDoubleTimeStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknDoubleTimeStyleListBox : public CAknDoubleTimeStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknDoubleTimeStyleListBoxDraw() { AccessCAknDoubleTimeStyleListBox * o = new (ELeave) AccessCAknDoubleTimeStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknDoubleTimeStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknDoubleTimeStyleListBox * o = reinterpret_cast< CAknDoubleTimeStyleListBox * >(control); Report(o, logger); }
void* GetCAknDoubleLargeStyleListBoxVTable() { CAknDoubleLargeStyleListBox * o = new (ELeave) CAknDoubleLargeStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknDoubleLargeStyleListBox : public CAknDoubleLargeStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknDoubleLargeStyleListBoxDraw() { AccessCAknDoubleLargeStyleListBox * o = new (ELeave) AccessCAknDoubleLargeStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknDoubleLargeStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknDoubleLargeStyleListBox * o = reinterpret_cast< CAknDoubleLargeStyleListBox * >(control); Report(o, logger); }
void* GetCAknDoubleGraphicStyleListBoxVTable() { CAknDoubleGraphicStyleListBox * o = new (ELeave) CAknDoubleGraphicStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknDoubleGraphicStyleListBox : public CAknDoubleGraphicStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknDoubleGraphicStyleListBoxDraw() { AccessCAknDoubleGraphicStyleListBox * o = new (ELeave) AccessCAknDoubleGraphicStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknDoubleGraphicStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknDoubleGraphicStyleListBox * o = reinterpret_cast< CAknDoubleGraphicStyleListBox * >(control); Report(o, logger); }
void* GetCAknDouble2GraphicStyleListBoxVTable() { CAknDouble2GraphicStyleListBox * o = new (ELeave) CAknDouble2GraphicStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknDouble2GraphicStyleListBox : public CAknDouble2GraphicStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknDouble2GraphicStyleListBoxDraw() { AccessCAknDouble2GraphicStyleListBox * o = new (ELeave) AccessCAknDouble2GraphicStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknDouble2GraphicStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknDouble2GraphicStyleListBox * o = reinterpret_cast< CAknDouble2GraphicStyleListBox * >(control); Report(o, logger); }
void* GetCAknDouble2LargeStyleListBoxVTable() { CAknDouble2LargeStyleListBox * o = new (ELeave) CAknDouble2LargeStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknDouble2LargeStyleListBox : public CAknDouble2LargeStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknDouble2LargeStyleListBoxDraw() { AccessCAknDouble2LargeStyleListBox * o = new (ELeave) AccessCAknDouble2LargeStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknDouble2LargeStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknDouble2LargeStyleListBox * o = reinterpret_cast< CAknDouble2LargeStyleListBox * >(control); Report(o, logger); }
void* GetCAknDouble2GraphicLargeGraphicStyleListBoxVTable() { CAknDouble2GraphicLargeGraphicStyleListBox * o = new (ELeave) CAknDouble2GraphicLargeGraphicStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknDouble2GraphicLargeGraphicStyleListBox : public CAknDouble2GraphicLargeGraphicStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknDouble2GraphicLargeGraphicStyleListBoxDraw() { AccessCAknDouble2GraphicLargeGraphicStyleListBox * o = new (ELeave) AccessCAknDouble2GraphicLargeGraphicStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknDouble2GraphicLargeGraphicStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknDouble2GraphicLargeGraphicStyleListBox * o = reinterpret_cast< CAknDouble2GraphicLargeGraphicStyleListBox * >(control); Report(o, logger); }
void* GetCAknFormDoubleGraphicStyleListBoxVTable() { CAknFormDoubleGraphicStyleListBox * o = new (ELeave) CAknFormDoubleGraphicStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknFormDoubleGraphicStyleListBox : public CAknFormDoubleGraphicStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknFormDoubleGraphicStyleListBoxDraw() { AccessCAknFormDoubleGraphicStyleListBox * o = new (ELeave) AccessCAknFormDoubleGraphicStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknFormDoubleGraphicStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknFormDoubleGraphicStyleListBox * o = reinterpret_cast< CAknFormDoubleGraphicStyleListBox * >(control); Report(o, logger); }
void* GetCAknFormDoubleStyleListBoxVTable() { CAknFormDoubleStyleListBox * o = new (ELeave) CAknFormDoubleStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknFormDoubleStyleListBox : public CAknFormDoubleStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknFormDoubleStyleListBoxDraw() { AccessCAknFormDoubleStyleListBox * o = new (ELeave) AccessCAknFormDoubleStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknFormDoubleStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknFormDoubleStyleListBox * o = reinterpret_cast< CAknFormDoubleStyleListBox * >(control); Report(o, logger); }
void* GetCAknSettingStyleListBoxVTable() { CAknSettingStyleListBox * o = new (ELeave) CAknSettingStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknSettingStyleListBox : public CAknSettingStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknSettingStyleListBoxDraw() { AccessCAknSettingStyleListBox * o = new (ELeave) AccessCAknSettingStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknSettingStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknSettingStyleListBox * o = reinterpret_cast< CAknSettingStyleListBox * >(control); Report(o, logger); }
void* GetCAknSettingNumberStyleListBoxVTable() { CAknSettingNumberStyleListBox * o = new (ELeave) CAknSettingNumberStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknSettingNumberStyleListBox : public CAknSettingNumberStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknSettingNumberStyleListBoxDraw() { AccessCAknSettingNumberStyleListBox * o = new (ELeave) AccessCAknSettingNumberStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknSettingNumberStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknSettingNumberStyleListBox * o = reinterpret_cast< CAknSettingNumberStyleListBox * >(control); Report(o, logger); }
void* GetCAknSinglePopupMenuStyleListBoxVTable() { CAknSinglePopupMenuStyleListBox * o = new (ELeave) CAknSinglePopupMenuStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknSinglePopupMenuStyleListBox : public CAknSinglePopupMenuStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknSinglePopupMenuStyleListBoxDraw() { AccessCAknSinglePopupMenuStyleListBox * o = new (ELeave) AccessCAknSinglePopupMenuStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknSinglePopupMenuStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknSinglePopupMenuStyleListBox * o = reinterpret_cast< CAknSinglePopupMenuStyleListBox * >(control); Report(o, logger); }
void* GetCAknSingleGraphicPopupMenuStyleListBoxVTable() { CAknSingleGraphicPopupMenuStyleListBox * o = new (ELeave) CAknSingleGraphicPopupMenuStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknSingleGraphicPopupMenuStyleListBox : public CAknSingleGraphicPopupMenuStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknSingleGraphicPopupMenuStyleListBoxDraw() { AccessCAknSingleGraphicPopupMenuStyleListBox * o = new (ELeave) AccessCAknSingleGraphicPopupMenuStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknSingleGraphicPopupMenuStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknSingleGraphicPopupMenuStyleListBox * o = reinterpret_cast< CAknSingleGraphicPopupMenuStyleListBox * >(control); Report(o, logger); }
void* GetCAknSingleGraphicBtPopupMenuStyleListBoxVTable() { CAknSingleGraphicBtPopupMenuStyleListBox * o = new (ELeave) CAknSingleGraphicBtPopupMenuStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknSingleGraphicBtPopupMenuStyleListBox : public CAknSingleGraphicBtPopupMenuStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknSingleGraphicBtPopupMenuStyleListBoxDraw() { AccessCAknSingleGraphicBtPopupMenuStyleListBox * o = new (ELeave) AccessCAknSingleGraphicBtPopupMenuStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknSingleGraphicBtPopupMenuStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknSingleGraphicBtPopupMenuStyleListBox * o = reinterpret_cast< CAknSingleGraphicBtPopupMenuStyleListBox * >(control); Report(o, logger); }
void* GetCAknSingleHeadingPopupMenuStyleListBoxVTable() { CAknSingleHeadingPopupMenuStyleListBox * o = new (ELeave) CAknSingleHeadingPopupMenuStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknSingleHeadingPopupMenuStyleListBox : public CAknSingleHeadingPopupMenuStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknSingleHeadingPopupMenuStyleListBoxDraw() { AccessCAknSingleHeadingPopupMenuStyleListBox * o = new (ELeave) AccessCAknSingleHeadingPopupMenuStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknSingleHeadingPopupMenuStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknSingleHeadingPopupMenuStyleListBox * o = reinterpret_cast< CAknSingleHeadingPopupMenuStyleListBox * >(control); Report(o, logger); }
void* GetCAknSingleGraphicHeadingPopupMenuStyleListBoxVTable() { CAknSingleGraphicHeadingPopupMenuStyleListBox * o = new (ELeave) CAknSingleGraphicHeadingPopupMenuStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknSingleGraphicHeadingPopupMenuStyleListBox : public CAknSingleGraphicHeadingPopupMenuStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknSingleGraphicHeadingPopupMenuStyleListBoxDraw() { AccessCAknSingleGraphicHeadingPopupMenuStyleListBox * o = new (ELeave) AccessCAknSingleGraphicHeadingPopupMenuStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknSingleGraphicHeadingPopupMenuStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknSingleGraphicHeadingPopupMenuStyleListBox * o = reinterpret_cast< CAknSingleGraphicHeadingPopupMenuStyleListBox * >(control); Report(o, logger); }
void* GetCAknDoublePopupMenuStyleListBoxVTable() { CAknDoublePopupMenuStyleListBox * o = new (ELeave) CAknDoublePopupMenuStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknDoublePopupMenuStyleListBox : public CAknDoublePopupMenuStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknDoublePopupMenuStyleListBoxDraw() { AccessCAknDoublePopupMenuStyleListBox * o = new (ELeave) AccessCAknDoublePopupMenuStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknDoublePopupMenuStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknDoublePopupMenuStyleListBox * o = reinterpret_cast< CAknDoublePopupMenuStyleListBox * >(control); Report(o, logger); }
void* GetCAknSinglePopupSubmenuStyleListBoxVTable() { CAknSinglePopupSubmenuStyleListBox * o = new (ELeave) CAknSinglePopupSubmenuStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknSinglePopupSubmenuStyleListBox : public CAknSinglePopupSubmenuStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknSinglePopupSubmenuStyleListBoxDraw() { AccessCAknSinglePopupSubmenuStyleListBox * o = new (ELeave) AccessCAknSinglePopupSubmenuStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknSinglePopupSubmenuStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknSinglePopupSubmenuStyleListBox * o = reinterpret_cast< CAknSinglePopupSubmenuStyleListBox * >(control); Report(o, logger); }
void* GetCAknDoubleLargeGraphicPopupMenuStyleListBoxVTable() { CAknDoubleLargeGraphicPopupMenuStyleListBox * o = new (ELeave) CAknDoubleLargeGraphicPopupMenuStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknDoubleLargeGraphicPopupMenuStyleListBox : public CAknDoubleLargeGraphicPopupMenuStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknDoubleLargeGraphicPopupMenuStyleListBoxDraw() { AccessCAknDoubleLargeGraphicPopupMenuStyleListBox * o = new (ELeave) AccessCAknDoubleLargeGraphicPopupMenuStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknDoubleLargeGraphicPopupMenuStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknDoubleLargeGraphicPopupMenuStyleListBox * o = reinterpret_cast< CAknDoubleLargeGraphicPopupMenuStyleListBox * >(control); Report(o, logger); }
void* GetCAknDouble2PopupMenuStyleListBoxVTable() { CAknDouble2PopupMenuStyleListBox * o = new (ELeave) CAknDouble2PopupMenuStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknDouble2PopupMenuStyleListBox : public CAknDouble2PopupMenuStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknDouble2PopupMenuStyleListBoxDraw() { AccessCAknDouble2PopupMenuStyleListBox * o = new (ELeave) AccessCAknDouble2PopupMenuStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknDouble2PopupMenuStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknDouble2PopupMenuStyleListBox * o = reinterpret_cast< CAknDouble2PopupMenuStyleListBox * >(control); Report(o, logger); }
void* GetCAknSingle2GraphicPopupMenuStyleListBoxVTable() { CAknSingle2GraphicPopupMenuStyleListBox * o = new (ELeave) CAknSingle2GraphicPopupMenuStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknSingle2GraphicPopupMenuStyleListBox : public CAknSingle2GraphicPopupMenuStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknSingle2GraphicPopupMenuStyleListBoxDraw() { AccessCAknSingle2GraphicPopupMenuStyleListBox * o = new (ELeave) AccessCAknSingle2GraphicPopupMenuStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknSingle2GraphicPopupMenuStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknSingle2GraphicPopupMenuStyleListBox * o = reinterpret_cast< CAknSingle2GraphicPopupMenuStyleListBox * >(control); Report(o, logger); }
void* GetCAknDoubleGraphicPopupMenuStyleListBoxVTable() { CAknDoubleGraphicPopupMenuStyleListBox * o = new (ELeave) CAknDoubleGraphicPopupMenuStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknDoubleGraphicPopupMenuStyleListBox : public CAknDoubleGraphicPopupMenuStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknDoubleGraphicPopupMenuStyleListBoxDraw() { AccessCAknDoubleGraphicPopupMenuStyleListBox * o = new (ELeave) AccessCAknDoubleGraphicPopupMenuStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknDoubleGraphicPopupMenuStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknDoubleGraphicPopupMenuStyleListBox * o = reinterpret_cast< CAknDoubleGraphicPopupMenuStyleListBox * >(control); Report(o, logger); }
void* GetCAknSetStyleListBoxVTable() { CAknSetStyleListBox * o = new (ELeave) CAknSetStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknSetStyleListBox : public CAknSetStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknSetStyleListBoxDraw() { AccessCAknSetStyleListBox * o = new (ELeave) AccessCAknSetStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknSetStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknSetStyleListBox * o = reinterpret_cast< CAknSetStyleListBox * >(control); Report(o, logger); }
void* GetCAknFormGraphicStyleListBoxVTable() { CAknFormGraphicStyleListBox * o = new (ELeave) CAknFormGraphicStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknFormGraphicStyleListBox : public CAknFormGraphicStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknFormGraphicStyleListBoxDraw() { AccessCAknFormGraphicStyleListBox * o = new (ELeave) AccessCAknFormGraphicStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknFormGraphicStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknFormGraphicStyleListBox * o = reinterpret_cast< CAknFormGraphicStyleListBox * >(control); Report(o, logger); }
void* GetCAknFormGraphicWideStyleListBoxVTable() { CAknFormGraphicWideStyleListBox * o = new (ELeave) CAknFormGraphicWideStyleListBox; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknFormGraphicWideStyleListBox : public CAknFormGraphicWideStyleListBox { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknFormGraphicWideStyleListBoxDraw() { AccessCAknFormGraphicWideStyleListBox * o = new (ELeave) AccessCAknFormGraphicWideStyleListBox; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknFormGraphicWideStyleListBoxState(CCoeControl* control, LoggingState* logger) { CAknFormGraphicWideStyleListBox * o = reinterpret_cast< CAknFormGraphicWideStyleListBox * >(control); Report(o, logger); }
void* GetCAknPinbStyleGridVTable() { CAknPinbStyleGrid * o = new (ELeave) CAknPinbStyleGrid; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknPinbStyleGrid : public CAknPinbStyleGrid { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknPinbStyleGridDraw() { AccessCAknPinbStyleGrid * o = new (ELeave) AccessCAknPinbStyleGrid; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknPinbStyleGridState(CCoeControl* control, LoggingState* logger) { CAknPinbStyleGrid * o = reinterpret_cast< CAknPinbStyleGrid * >(control); Report(o, logger); }
void* GetCAknQdialStyleGridVTable() { CAknQdialStyleGrid * o = new (ELeave) CAknQdialStyleGrid; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknQdialStyleGrid : public CAknQdialStyleGrid { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknQdialStyleGridDraw() { AccessCAknQdialStyleGrid * o = new (ELeave) AccessCAknQdialStyleGrid; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknQdialStyleGridState(CCoeControl* control, LoggingState* logger) { CAknQdialStyleGrid * o = reinterpret_cast< CAknQdialStyleGrid * >(control); Report(o, logger); }
void* GetCAknCaleMonthStyleGridVTable() { CAknCaleMonthStyleGrid * o = new (ELeave) CAknCaleMonthStyleGrid; void* ptr = *(void**)o; delete o; return ptr; } class AccessCAknCaleMonthStyleGrid : public CAknCaleMonthStyleGrid { public: DrawFunc GetDraw() { return &Draw; } }; DrawFunc GetCAknCaleMonthStyleGridDraw() { AccessCAknCaleMonthStyleGrid * o = new (ELeave) AccessCAknCaleMonthStyleGrid; DrawFunc ptr = o->GetDraw(); delete o; return ptr; } void ReportCAknCaleMonthStyleGridState(CCoeControl* control, LoggingState* logger) { CAknCaleMonthStyleGrid * o = reinterpret_cast< CAknCaleMonthStyleGrid * >(control); Report(o, logger); }
# 84 "control_walker.cpp" 2
# 93 "control_walker.cpp"
ClassSig classes[] = {
# 1 "control_classes.h" 1




{ (const unsigned char*)"CEikLabel", & GetCEikLabelVTable, & GetCEikLabelDraw, & ReportCEikLabelState },
{ (const unsigned char*)"CAknColumnListBox", & GetCAknColumnListBoxVTable, & GetCAknColumnListBoxDraw, & ReportCAknColumnListBoxState },
{ (const unsigned char*)"CAknSingleStyleListBox", & GetCAknSingleStyleListBoxVTable, & GetCAknSingleStyleListBoxDraw, & ReportCAknSingleStyleListBoxState },
{ (const unsigned char*)"CAknSingleNumberStyleListBox", & GetCAknSingleNumberStyleListBoxVTable, & GetCAknSingleNumberStyleListBoxDraw, & ReportCAknSingleNumberStyleListBoxState },
{ (const unsigned char*)"CAknSingleHeadingStyleListBox", & GetCAknSingleHeadingStyleListBoxVTable, & GetCAknSingleHeadingStyleListBoxDraw, & ReportCAknSingleHeadingStyleListBoxState },
{ (const unsigned char*)"CAknSingleGraphicStyleListBox", & GetCAknSingleGraphicStyleListBoxVTable, & GetCAknSingleGraphicStyleListBoxDraw, & ReportCAknSingleGraphicStyleListBoxState },
{ (const unsigned char*)"CAknSingleGraphicHeadingStyleListBox", & GetCAknSingleGraphicHeadingStyleListBoxVTable, & GetCAknSingleGraphicHeadingStyleListBoxDraw, & ReportCAknSingleGraphicHeadingStyleListBoxState },
{ (const unsigned char*)"CAknSingleNumberHeadingStyleListBox", & GetCAknSingleNumberHeadingStyleListBoxVTable, & GetCAknSingleNumberHeadingStyleListBoxDraw, & ReportCAknSingleNumberHeadingStyleListBoxState },
{ (const unsigned char*)"CAknSingleLargeStyleListBox", & GetCAknSingleLargeStyleListBoxVTable, & GetCAknSingleLargeStyleListBoxDraw, & ReportCAknSingleLargeStyleListBoxState },
{ (const unsigned char*)"CAknDoubleStyleListBox", & GetCAknDoubleStyleListBoxVTable, & GetCAknDoubleStyleListBoxDraw, & ReportCAknDoubleStyleListBoxState },
{ (const unsigned char*)"CAknDoubleStyle2ListBox", & GetCAknDoubleStyle2ListBoxVTable, & GetCAknDoubleStyle2ListBoxDraw, & ReportCAknDoubleStyle2ListBoxState },
{ (const unsigned char*)"CAknDoubleNumberStyleListBox", & GetCAknDoubleNumberStyleListBoxVTable, & GetCAknDoubleNumberStyleListBoxDraw, & ReportCAknDoubleNumberStyleListBoxState },
{ (const unsigned char*)"CAknDoubleTimeStyleListBox", & GetCAknDoubleTimeStyleListBoxVTable, & GetCAknDoubleTimeStyleListBoxDraw, & ReportCAknDoubleTimeStyleListBoxState },
{ (const unsigned char*)"CAknDoubleLargeStyleListBox", & GetCAknDoubleLargeStyleListBoxVTable, & GetCAknDoubleLargeStyleListBoxDraw, & ReportCAknDoubleLargeStyleListBoxState },
{ (const unsigned char*)"CAknDoubleGraphicStyleListBox", & GetCAknDoubleGraphicStyleListBoxVTable, & GetCAknDoubleGraphicStyleListBoxDraw, & ReportCAknDoubleGraphicStyleListBoxState },
{ (const unsigned char*)"CAknDouble2GraphicStyleListBox", & GetCAknDouble2GraphicStyleListBoxVTable, & GetCAknDouble2GraphicStyleListBoxDraw, & ReportCAknDouble2GraphicStyleListBoxState },
{ (const unsigned char*)"CAknDouble2LargeStyleListBox", & GetCAknDouble2LargeStyleListBoxVTable, & GetCAknDouble2LargeStyleListBoxDraw, & ReportCAknDouble2LargeStyleListBoxState },
{ (const unsigned char*)"CAknDouble2GraphicLargeGraphicStyleListBox", & GetCAknDouble2GraphicLargeGraphicStyleListBoxVTable, & GetCAknDouble2GraphicLargeGraphicStyleListBoxDraw, & ReportCAknDouble2GraphicLargeGraphicStyleListBoxState },
{ (const unsigned char*)"CAknFormDoubleGraphicStyleListBox", & GetCAknFormDoubleGraphicStyleListBoxVTable, & GetCAknFormDoubleGraphicStyleListBoxDraw, & ReportCAknFormDoubleGraphicStyleListBoxState },
{ (const unsigned char*)"CAknFormDoubleStyleListBox", & GetCAknFormDoubleStyleListBoxVTable, & GetCAknFormDoubleStyleListBoxDraw, & ReportCAknFormDoubleStyleListBoxState },
{ (const unsigned char*)"CAknSettingStyleListBox", & GetCAknSettingStyleListBoxVTable, & GetCAknSettingStyleListBoxDraw, & ReportCAknSettingStyleListBoxState },
{ (const unsigned char*)"CAknSettingNumberStyleListBox", & GetCAknSettingNumberStyleListBoxVTable, & GetCAknSettingNumberStyleListBoxDraw, & ReportCAknSettingNumberStyleListBoxState },
{ (const unsigned char*)"CAknSinglePopupMenuStyleListBox", & GetCAknSinglePopupMenuStyleListBoxVTable, & GetCAknSinglePopupMenuStyleListBoxDraw, & ReportCAknSinglePopupMenuStyleListBoxState },
{ (const unsigned char*)"CAknSingleGraphicPopupMenuStyleListBox", & GetCAknSingleGraphicPopupMenuStyleListBoxVTable, & GetCAknSingleGraphicPopupMenuStyleListBoxDraw, & ReportCAknSingleGraphicPopupMenuStyleListBoxState },
{ (const unsigned char*)"CAknSingleGraphicBtPopupMenuStyleListBox", & GetCAknSingleGraphicBtPopupMenuStyleListBoxVTable, & GetCAknSingleGraphicBtPopupMenuStyleListBoxDraw, & ReportCAknSingleGraphicBtPopupMenuStyleListBoxState },
{ (const unsigned char*)"CAknSingleHeadingPopupMenuStyleListBox", & GetCAknSingleHeadingPopupMenuStyleListBoxVTable, & GetCAknSingleHeadingPopupMenuStyleListBoxDraw, & ReportCAknSingleHeadingPopupMenuStyleListBoxState },
{ (const unsigned char*)"CAknSingleGraphicHeadingPopupMenuStyleListBox", & GetCAknSingleGraphicHeadingPopupMenuStyleListBoxVTable, & GetCAknSingleGraphicHeadingPopupMenuStyleListBoxDraw, & ReportCAknSingleGraphicHeadingPopupMenuStyleListBoxState },
{ (const unsigned char*)"CAknDoublePopupMenuStyleListBox", & GetCAknDoublePopupMenuStyleListBoxVTable, & GetCAknDoublePopupMenuStyleListBoxDraw, & ReportCAknDoublePopupMenuStyleListBoxState },
{ (const unsigned char*)"CAknSinglePopupSubmenuStyleListBox", & GetCAknSinglePopupSubmenuStyleListBoxVTable, & GetCAknSinglePopupSubmenuStyleListBoxDraw, & ReportCAknSinglePopupSubmenuStyleListBoxState },
{ (const unsigned char*)"CAknDoubleLargeGraphicPopupMenuStyleListBox", & GetCAknDoubleLargeGraphicPopupMenuStyleListBoxVTable, & GetCAknDoubleLargeGraphicPopupMenuStyleListBoxDraw, & ReportCAknDoubleLargeGraphicPopupMenuStyleListBoxState },
{ (const unsigned char*)"CAknDouble2PopupMenuStyleListBox", & GetCAknDouble2PopupMenuStyleListBoxVTable, & GetCAknDouble2PopupMenuStyleListBoxDraw, & ReportCAknDouble2PopupMenuStyleListBoxState },
{ (const unsigned char*)"CAknSingle2GraphicPopupMenuStyleListBox", & GetCAknSingle2GraphicPopupMenuStyleListBoxVTable, & GetCAknSingle2GraphicPopupMenuStyleListBoxDraw, & ReportCAknSingle2GraphicPopupMenuStyleListBoxState },
{ (const unsigned char*)"CAknDoubleGraphicPopupMenuStyleListBox", & GetCAknDoubleGraphicPopupMenuStyleListBoxVTable, & GetCAknDoubleGraphicPopupMenuStyleListBoxDraw, & ReportCAknDoubleGraphicPopupMenuStyleListBoxState },
{ (const unsigned char*)"CAknSetStyleListBox", & GetCAknSetStyleListBoxVTable, & GetCAknSetStyleListBoxDraw, & ReportCAknSetStyleListBoxState },
{ (const unsigned char*)"CAknFormGraphicStyleListBox", & GetCAknFormGraphicStyleListBoxVTable, & GetCAknFormGraphicStyleListBoxDraw, & ReportCAknFormGraphicStyleListBoxState },
{ (const unsigned char*)"CAknFormGraphicWideStyleListBox", & GetCAknFormGraphicWideStyleListBoxVTable, & GetCAknFormGraphicWideStyleListBoxDraw, & ReportCAknFormGraphicWideStyleListBoxState },
{ (const unsigned char*)"CAknPinbStyleGrid", & GetCAknPinbStyleGridVTable, & GetCAknPinbStyleGridDraw, & ReportCAknPinbStyleGridState },
{ (const unsigned char*)"CAknQdialStyleGrid", & GetCAknQdialStyleGridVTable, & GetCAknQdialStyleGridDraw, & ReportCAknQdialStyleGridState },
{ (const unsigned char*)"CAknCaleMonthStyleGrid", & GetCAknCaleMonthStyleGridVTable, & GetCAknCaleMonthStyleGridDraw, & ReportCAknCaleMonthStyleGridState },
# 95 "control_walker.cpp" 2
  { 0 },
};



void InspectControl(CCoeControl* control, LoggingState* logger) {
  void* vtable = *(void**)control;
  for (const ClassSig* control_class = classes;
       control_class->class_name;
       ++control_class) {
    if (vtable == (*(control_class->GetVTableAddress))() ||
        &control->Draw == (*(control_class->GetDrawAddress))()) {
      TBuf<100> name;
      name.Copy(TPtrC8(control_class->class_name));
      logger->Log(name);
      (*(control_class->ReportState))(control, logger);
      return;
    }
  }
  logger->Log(_L("unknown"));
}

void WalkOne(CCoeControl* control, LoggingState* logger) {
  if (!control) return;
  InspectControl(control, logger);
  const int child_count = control->CountComponentControls();
  if (child_count == 0) return;
  logger->IncreaseIndent();
  TBuf<30> buf= _L("children ");
  buf.AppendNum(child_count);
  logger->Log(buf);
  for (int i = 0; i < child_count; ++i) {
    WalkOne(control->ComponentControl(i), logger);
  }
  logger->DecreaseIndent();
}
}

ControlWalker::ControlWalker() : CActive(CActive::EPriorityLow) {
  CActiveScheduler::Add(this);
}

ControlWalker::~ControlWalker() {
  Cancel();
}

void ControlWalker::TriggerWalk(LoggingState* logger) {
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
  CCoeControl* top = env->AppUi()->TopFocusedControl();
  WalkOne(top, logger);
}
