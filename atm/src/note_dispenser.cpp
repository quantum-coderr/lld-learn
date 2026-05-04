#include "../include/note_dispenser.h"
#include <iostream>
#include <stdexcept>

void NoteDispenser::dispense(int amount) {
  if (amount <= 0) {
    if (nextHandler) {
      nextHandler->dispense(amount);
    }
    return;
  }

  int numNotes = amount / noteValue;
  int remainder = amount % noteValue;

  if (numNotes > 0) {
    std::cout << "Dispensing " << numNotes << " note(s) of ₹" << noteValue
              << std::endl;
  }

  if (remainder != 0) {
    if (nextHandler) {
      std::cout << "Remaining ₹" << remainder << " passed to next handler..."
                << std::endl;
      nextHandler->dispense(remainder);
    } else {
      // This is the last handler and we still have remainder → cannot dispense
      throw std::runtime_error("Cannot dispense remaining amount: ₹" +
                               std::to_string(remainder));
    }
  }
  // If remainder == 0, chain ends successfully
}