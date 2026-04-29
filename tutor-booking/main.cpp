#include "logger/Logger.h"
#include "slot/TimeSlot.h"
#include "system/TutorBookingSystem.h"
#include "user/UserFactory.h"
#include <vector>

int main() {

  LOG.separator("Tutor Booking System — Demo");

  // ── Setup ──────────────────────────────────────────────────────────────────
  LOG.info("main", "Setting up users and slots...");

  Student *student = UserFactory::createStudent(1, "Rohan");
  Tutor *tutor = UserFactory::createTutor(2, "Yash", "Math");
  TimeSlot *slot = new TimeSlot(1, "10:00", "11:00");

  tutor->addSlot(slot);

  std::vector<Tutor *> tutors = {tutor};
  TutorBookingSystem system;

  // ── Booking 1 (should succeed) ─────────────────────────────────────────────
  LOG.info("main", "Attempting Booking #1...");
  Booking *b1 = system.bookTutor(student, tutors, "Math");

  if (b1)
    LOG.success("main", "Result → Booking Successful ✔");
  else
    LOG.error("main", "Result → Booking Failed ✘");

  // ── Booking 2 (should fail — slot already taken) ───────────────────────────
  LOG.info("main", "Attempting Booking #2 (same slot, should fail)...");
  Booking *b2 = system.bookTutor(student, tutors, "Math");

  if (b2)
    LOG.success("main", "Result → Booking Successful ✔");
  else
    LOG.error("main", "Result → Booking Failed ✘");

  LOG.separator("Session Complete");

  return 0;
}