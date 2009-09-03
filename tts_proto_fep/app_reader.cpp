#include "app_reader.h"

AppState::~AppState() {
  delete title_;
  delete search_field_;
  delete first_softkey_;
  delete second_softkey_;
  delete item_text_;
  delete debug_;
}

void AppState::Reset() {
  app_uid_ = TUid::Uid(0);
  view_uid_ = TUid::Uid(0);
  ResetText(title_);
  ResetText(search_field_);
  ResetText(first_softkey_);
  ResetText(second_softkey_);
  menu_or_popup_ = EFalse;
  ResetText(item_text_);
  item_index_ = -1;
  item_count_ = -1;
  ResetText(debug_);
}
