#pragma once

#include "../slot/TimeSlot.h"
#include "../user/Student.h"
#include "../user/Tutor.h"

enum class BookingStatus { CREATED, CONFIRMED, CANCELLED };

class Booking {

private:
  int id;
  Student *student;
  Tutor *tutor;
  TimeSlot *slot;
  BookingStatus status;

public:
  Booking(int id, Student *s, Tutor *t, TimeSlot *slot)
      : id(id), student(s), tutor(t), slot(slot),
        status(BookingStatus::CREATED) {}

  int getId() { return id; }

  BookingStatus getStatus() { return this->status; }

  void cancel() { this->status = BookingStatus::CANCELLED; }
};