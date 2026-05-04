#include "../include/cash_dispenser.h"
#include "../include/note_dispenser.h"
#include <iostream>

CashDispenser::CashDispenser() { buildChain(); }

void CashDispenser::buildChain() {
  // Order is important - highest denomination first
  auto dispenser2000 = std::make_unique<NoteDispenser>(2000);
  auto dispenser500 = std::make_unique<NoteDispenser>(500);
  auto dispenser200 = std::make_unique<NoteDispenser>(200);
  auto dispenser100 = std::make_unique<NoteDispenser>(100);
  auto dispenser50 = std::make_unique<NoteDispenser>(50);
  auto dispenser20 = std::make_unique<NoteDispenser>(20);

  // Link them bottom-up: build from tail → head so each ptr is still valid
  // when setNext() is called on it (avoids use-after-move UB).
  dispenser50->setNext(std::move(dispenser20));
  dispenser100->setNext(std::move(dispenser50));
  dispenser200->setNext(std::move(dispenser100));
  dispenser500->setNext(std::move(dispenser200));
  dispenser2000->setNext(std::move(dispenser500));

  // Head of the chain
  chainHead = std::move(dispenser2000);
}

void CashDispenser::dispenseCash(int amount) {
  if (amount <= 0) {
    throw std::invalid_argument("Amount must be positive");
  }
  if (amount % 10 != 0) { // Assuming ATM only supports multiples of 10
    throw std::invalid_argument("Amount must be a multiple of 10");
  }

  std::cout << "\n=== Attempting to dispense ₹" << amount << " ===\n";

  try {
    chainHead->dispense(amount);
    std::cout << "=== Cash dispensed successfully ===\n";
  } catch (const std::exception &e) {
    std::cout << "Dispense failed: " << e.what() << std::endl;
    throw; // rethrow so ATM can handle it
  }
}

bool CashDispenser::canDispense(int amount) const {
  // In a real ATM we would track actual note inventory.
  // For learning, we'll assume it can always dispense if amount is valid.
  return amount > 0 && amount % 10 == 0;
}