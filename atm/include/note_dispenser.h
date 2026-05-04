#pragma once
#include "dispense_handler.h"

class NoteDispenser : public DispenseHandler {
private:
  const int noteValue;

public:
  explicit NoteDispenser(int value) : noteValue(value) {}

  int getNoteValue() const override { return noteValue; }

  void dispense(int amount) override;
};