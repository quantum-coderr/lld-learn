#pragma once

#include "Booking.h"
#include <vector>

class BookingRepository {

private:
  std::vector<Booking *> bookings;

public:
  void addBooking(Booking *booking) { bookings.push_back(booking); }

  std::vector<Booking *> &getBookings() { return bookings; }
};