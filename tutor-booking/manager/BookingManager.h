#pragma once
#include "../booking/Booking.h"
#include "../booking/BookingRepository.h"
#include "../logger/Logger.h"

// ─────────────────────────────────────────────────────────────────────────────
// BookingManager  –  orchestrates booking creation & cancellation
// Repository Pattern: persists bookings via BookingRepository
// Single Responsibility: manages booking lifecycle only
// ─────────────────────────────────────────────────────────────────────────────
class BookingManager {
private:
  int bookingCounter = 1;
  BookingRepository repository;

public:
  Booking *createBooking(Student *student, Tutor *tutor, TimeSlot *slot) {
    LOG.info("BookingManager",
             "Attempting booking → Student: " + student->getName() +
                 " | Tutor: " + tutor->getName() +
                 " | Slot: [" + slot->getStartTime() + " – " + slot->getEndTime() + "]");

    if (!slot->isAvailable()) {
      LOG.error("BookingManager",
                "Slot [" + slot->getStartTime() + " – " + slot->getEndTime() +
                    "] is not available. Booking rejected.");
      return nullptr;
    }

    slot->bookSlot();
    Booking *booking = new Booking(bookingCounter++, student, tutor, slot);
    repository.addBooking(booking);

    LOG.success("BookingManager",
                "Booking #" + std::to_string(booking->getId()) +
                    " created for " + student->getName() +
                    " with tutor " + tutor->getName());
    return booking;
  }

  void cancelBooking(Booking *booking) {
    LOG.warn("BookingManager", "Cancelling booking #" + std::to_string(booking->getId()));
    booking->cancel();
    LOG.info("BookingManager", "Booking #" + std::to_string(booking->getId()) + " cancelled.");
  }

  std::vector<Booking *> &getAllBookings() { return repository.getBookings(); }
};