#include "../include/cash_dispenser.h"
#include <iostream>

int main() {
  try {
    CashDispenser dispenser;

    std::cout << "=== Test 1: ₹2000 ===\n";
    dispenser.dispenseCash(2000);

    std::cout << "\n=== Test 2: ₹750 ===\n";
    dispenser.dispenseCash(750);

    std::cout << "\n=== Test 3: ₹1230 ===\n";
    dispenser.dispenseCash(1230);

    std::cout << "\n=== Test 4: Invalid Amount ===\n";
    dispenser.dispenseCash(125);

  } catch (const std::exception &e) {
    std::cout << "Error: " << e.what() << std::endl;
  }

  return 0;
}