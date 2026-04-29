#pragma once

#include "../logger/Logger.h"
#include "../manager/BookingManager.h"
#include "../manager/SlotManager.h"
#include "../search/SearchService.h"

// ─────────────────────────────────────────────────────────────────────────────
// TutorBookingSystem  –  Facade Pattern
// Provides a unified, simple interface to the subsystems:
//   SearchService → SlotManager → BookingManager
// Demonstrates: Dependency Inversion (composes abstractions, not concretions)
//               Single Responsibility (orchestration only)
// ─────────────────────────────────────────────────────────────────────────────
class TutorBookingSystem {

private:
  SearchService searchService;
  SlotManager slotManager;
  BookingManager bookingManager;

public:
  Booking *bookTutor(Student *student, std::vector<Tutor *> &tutors,
                     std::string subject) {

    LOG.separator("Booking Request — " + student->getName() + " → " + subject);

    // Step 1: Search
    auto result = searchService.searchBySubject(tutors, subject);
    if (result.empty()) {
      LOG.error("TutorBookingSystem",
                "No tutor found for subject \"" + subject + "\". Aborting.");
      return nullptr;
    }

    Tutor *tutor = result[0];
    LOG.info("TutorBookingSystem", "Selected tutor: " + tutor->getName());

    // Step 2: Find available slot
    TimeSlot *slot = slotManager.findAvailableSlot(tutor);
    if (!slot) {
      LOG.error("TutorBookingSystem",
                "No available slot for tutor " + tutor->getName() + ". Aborting.");
      return nullptr;
    }

    // Step 3: Create booking
    Booking *booking = bookingManager.createBooking(student, tutor, slot);
    if (booking) {
      LOG.success("TutorBookingSystem",
                  "✔ Booking #" + std::to_string(booking->getId()) + " completed successfully!\n");
    } else {
      LOG.error("TutorBookingSystem", "✘ Booking failed.\n");
    }
    return booking;
  }
};