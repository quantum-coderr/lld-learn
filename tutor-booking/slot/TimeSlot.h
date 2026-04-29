#pragma once
#include "../logger/Logger.h"
#include <mutex>
#include <string>
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// TimeSlot  –  represents a bookable time window for a tutor
// Thread-safe slot locking via std::mutex (mutex guard pattern)
// ─────────────────────────────────────────────────────────────────────────────
class TimeSlot {
private:
  int id;
  string startTime;
  string endTime;
  bool booked;
  mutex mtx;

public:
  TimeSlot(int id, string startTime, string endTime)
      : id(id), startTime(startTime), endTime(endTime), booked(false) {
    LOG.debug("TimeSlot", "Created slot #" + to_string(id) +
                              " [" + startTime + " – " + endTime + "]");
  }

  bool isAvailable() { return !this->booked; }

  bool bookSlot() {
    std::lock_guard<std::mutex> lock(mtx);
    LOG.debug("TimeSlot", "Attempting to book slot #" + to_string(id));
    if (booked) {
      LOG.warn("TimeSlot", "Slot #" + to_string(id) + " is already booked!");
      return false;
    }
    booked = true;
    LOG.success("TimeSlot", "Slot #" + to_string(id) + " [" + startTime +
                                " – " + endTime + "] successfully booked");
    return true;
  }

  int getId() const { return this->id; }
  string getStartTime() const { return this->startTime; }
  string getEndTime() const { return this->endTime; }
};