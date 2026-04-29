#pragma once
#include "../logger/Logger.h"
#include "../user/Tutor.h"
#include <cstddef>

// ─────────────────────────────────────────────────────────────────────────────
// SlotManager  –  Single Responsibility: find available slots only
// ─────────────────────────────────────────────────────────────────────────────
class SlotManager {
public:
  TimeSlot *findAvailableSlot(Tutor *t) {
    LOG.info("SlotManager", "Scanning slots for tutor: " + t->getName());
    for (auto slot : t->getSlots()) {
      if (slot->isAvailable()) {
        LOG.success("SlotManager", "Available slot found → [" +
                                       slot->getStartTime() + " – " +
                                       slot->getEndTime() + "]");
        return slot;
      }
    }
    LOG.warn("SlotManager", "No available slot found for tutor: " + t->getName());
    return nullptr;
  }
};