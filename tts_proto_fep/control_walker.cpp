#include "control_walker.h"

#include <eikappui.h>
#include <eikedwin.h>
#include <eikenv.h>

#include "reporting.h"

namespace {

// We want to be able detect known control class instances with the minimum
// fuss, so we we write the necessary reporting functions and generate a lookup
// table.
// For the technique http://blog.brush.co.nz/2009/08/xmacros/

void Report(CEikEdwin* edwin, LoggingState* logger) {
  HBufC* text = edwin->GetTextInHBufL();
  CleanupStack::PushL(text);
  logger->Log(*text);
  CleanupStack::PopAndDestroy(text);
}

struct ClassSig {
 const unsigned char* class_name;
 void* (*GetVTableAddress)(void);
 void (*ReportState)(CCoeControl* control, LoggingState* logger);
};

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
  { TEXT(name), & Get ## name ## VTable, & Report ## name ## State },

ClassSig classes[] = {
#include "control_classes.h"
  { 0 },
};
#undef CONTROL
#undef TEXT

void InspectControl(CCoeControl* control, LoggingState* logger) {
  void* vtable = *(void**)control;
  for (const ClassSig* control_class = classes;
       control_class->class_name;
       ++control_class) {
    if (vtable == (*(control_class->GetVTableAddress))()) {
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
  InspectControl(control, logger);
  logger->Log(_L("children"));
  for (int i = 0; i < control->CountComponentControls(); ++i) {
    WalkOne(control->ComponentControl(i), logger);
  }
}
}  // namespace

ControlWalker::ControlWalker() {}

ControlWalker::~ControlWalker() {}

void ControlWalker::Walk(LoggingState* logger) {
  CEikonEnv* env = CEikonEnv::Static();
  CCoeControl* top = env->AppUi()->TopFocusedControl();
  WalkOne(top, logger);
}
