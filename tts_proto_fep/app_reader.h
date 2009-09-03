#ifndef APP_READER_H_
#define APP_READER_H_

#include <e32base.h>

// This is a first draft of structured state reporting. It may make sense
// to split up and/or create a class hierarchy for the data later.
class AppState : public CBase {
 public:
  AppState() {
    // CBase members are zero-initialized.
  }
  ~AppState();
  const TUid& AppUid() const { return app_uid_; }
  const TUid& ViewUid() const { return view_uid_; }
  const TDesC& Title() const { return ReturnText(title_); }
  const TDesC& SearchFieldText() const { return ReturnText(search_field_); }
  const TDesC& FirstSoftkey() const { return ReturnText(first_softkey_); }
  const TDesC& SecondSoftkey() const { return ReturnText(second_softkey_); }
  TBool IsShowingMenuOrPopup() const { return menu_or_popup_; }
  // Either list, grid or menu. Index and Count return -1 on unknown.
  const TDesC& SelectedItemText() const { return ReturnText(item_text_); }
  const TInt SelectedItemIndex() const { return item_index_; }
  const TInt ItemCount() const { return item_count_; }

  void Reset();
  void SetAppUid(const TUid& v) { app_uid_ = v; }
  void SetViewUid(const TUid& v) { view_uid_ = v; }
  void SetTitle(const TDesC& v) { SetText(&title_, v); }
  void SetSearchFieldText(const TDesC& v) { SetText(&search_field_, v); }
  void SetFirstSoftkey(const TDesC& v) { SetText(&first_softkey_, v); }
  void SetSecondSoftkey(const TDesC& v) { SetText(&second_softkey_, v); }
  void SetIsShowingMenuOrPopup(TBool v) { menu_or_popup_ = v; }
  void SetSelectedItemText(const TDesC& v) { SetText(&item_text_, v); }
  void SetSelectedItemIndex(const TInt v) { item_index_ = v; }
  void SetItemCount(const TInt v) { item_count_ = v; }

 private:
  const TDesC& ReturnText(HBufC* field) const {
    if (!field) return KNullDesC;
    return *field;
  }
  void SetText(HBufC** field, const TDesC& text) {
    if (!*field) *field = HBufC::NewL(50);
    *field = (*field)->ReAllocL(text.Length());
    (*field)->Des() = text;
  }
  void ResetText(HBufC* field) {
    if (!field) return;
    field->Des().Zero();
  }
  TUid app_uid_;
  TUid view_uid_;
  HBufC* title_;
  HBufC* search_field_;
  HBufC* first_softkey_;
  HBufC* second_softkey_;
  TBool menu_or_popup_;
  HBufC* item_text_;
  TInt item_index_;
  TInt item_count_;
};

class AppReaderInterface {
 public:
  AppReaderInterface() {}
  virtual ~AppReaderInterface() {}
  virtual const TUid& ForApplication() const = 0;
  virtual void Read() = 0;
  virtual const AppState& State() const = 0;
};

#endif  // APP_READER_H_
