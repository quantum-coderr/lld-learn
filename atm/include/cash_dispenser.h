#pragma once
#include "dispense_handler.h"
#include <memory>
#include <stdexcept>

class CashDispenser {
private:
  std::unique_ptr<DispenseHandler> chainHead;

  // Helper to build the chain (highest to lowest)
  void buildChain();

public:
  CashDispenser();

  // Main public API
  void dispenseCash(int amount);

  // For future extensibility (e.g., check if ATM has enough cash)
  bool canDispense(int amount) const;
};