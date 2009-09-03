#ifndef RTTI_H_
#define RTTI_H_

#include <e32base.h>

class CAknGrid;
class CAknSearchField;
class CCoeControl;
class CEikEdwin;
class CEikLabel;
class CEikListBox;
class CEikMenuPane;
class CEikTextListBox;

// 'RTTI': classes for determining or guessing the class of objects at
// run-time.

// TODO(mikie): cache all the vtable addresses. Will be done as the methods
// are used in non-development-mode code.

// IsValidPointer checks that the given address is word-aligned and within
// the thread heap.
bool IsValidPointer(void* p);

// SafeTypes determines safely and guaranteed accurately the class of
// controls. It typically does this by comparing vtable pointers.
class SafeTypes : public CBase{
 public:
  SafeTypes();
  CAknSearchField* IsAknSearchField(CCoeControl* control);
  CEikEdwin* IsEikEdwin(CCoeControl* control);
  CEikLabel* IsEikLabel(CCoeControl* control);
  CEikMenuPane* IsEikMenuPane(CCoeControl* control);
 private:
  void* menu_pane_vtable_;
};

// UnsafeTypes uses heuristics to make a good guess about the type of the
// control passed in but may be wrong.
class UnsafeTypes : public CBase {
 public:
  CEikTextListBox* IsEikTextListBox(CCoeControl* control);
  CEikListBox* IsEikListBox(CCoeControl* control);
  CAknGrid* IsAknGrid(CCoeControl* control);
};

#endif  // RTTI_H_
