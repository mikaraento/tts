#include "app_readers/phone.h"

namespace {
const TUid kPhoneUid = { 0x100058b3 };
}  // namespace

const TUid& PhoneReader::ForApplication() const {
  return kPhoneUid;
}

void PhoneReader::Read() {
  app_state_.Reset();
  app_helper_.ReadTitle(&control_tree_, &app_state_);
  const TVwsViewId view_id = app_helper_.ReadView(&control_tree_, &app_state_);
  // Calling app_helper_.ReadCba() will cause a KERN-EXEC 3 crash later when
  // keys are pressed. Have not tested other helpers.
  return;
}
