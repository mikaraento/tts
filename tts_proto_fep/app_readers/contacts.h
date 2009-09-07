#ifndef APP_READERS_CONTACTS_H_
#define APP_READERS_CONTACTS_H_

#include "app_helper.h"
#include "app_reader.h"
#include "control_tree.h"
#include "rtti.h"

class CEikListBox;

// Reads the Contacts app.
class ContactsReader : public CBase, public AppReaderInterface {
 public:
  ContactsReader() {}
  virtual ~ContactsReader() {}
  virtual const TUid& ForApplication() const;
  virtual void Read();
  virtual const AppState& State() const { return app_state_; }

 private:
  void ReadList(CEikListBox* listbox, AppState* app_state);
  AppState app_state_;
  ControlTree control_tree_;
  AppHelper app_helper_;
  SafeTypes safe_;
};

#endif  // APP_READERS_CONTACTS_H_
