#include "trigger.h"

AsyncTrigger::AsyncTrigger(TriggerInterface* inner)
    : CActive(CActive::EPriorityStandard),
      inner_(inner) {
  CActiveScheduler::Add(this);
}

void AsyncTrigger::OnTrigger() {
  if (IsActive()) return;
  TRequestStatus* s = &iStatus;
  User::RequestComplete(s, KErrNone);
  SetActive();
}

void AsyncTrigger::DoCancel() { }

void AsyncTrigger::RunL() {
  inner_->OnTrigger();
}

AsyncTrigger::~AsyncTrigger() {
  Cancel();
}
