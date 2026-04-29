#pragma once
#include "../logger/Logger.h"
#include "../slot/TimeSlot.h"
#include "User.h"
#include <string>
#include <vector>
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// Tutor  –  extends User (Inheritance / Liskov Substitution Principle)
// ─────────────────────────────────────────────────────────────────────────────
class Tutor : public User {
private:
  vector<TimeSlot *> slots;
  string subject;

public:
  Tutor(int id, string name, string s) : User(id, name), subject(s) {
    LOG.info("Tutor", "Registered tutor: " + name + " | Subject: " + s);
  }

  void addSlot(TimeSlot *t) {
    slots.push_back(t);
    LOG.info("Tutor", name + " → added slot [" + t->getStartTime() +
                          " – " + t->getEndTime() + "]");
  }

  std::vector<TimeSlot *> &getSlots() { return slots; }

  string getSubject() { return this->subject; }
};