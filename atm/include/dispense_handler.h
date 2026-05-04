#pragma once
#include <cstddef>
#include <iostream>
#include <memory>

class DispenseHandler {
protected:
  std::unique_ptr<DispenseHandler> nextHandler = nullptr;

public:
  virtual ~DispenseHandler() = default;

  // COR core method
  virtual void dispense(int amount) = 0;

  // Helper to link the chain
  void setNext(std::unique_ptr<DispenseHandler> next) {
    nextHandler = std::move(next);
  }

  // Utility to check if this handler can dispense any notes of its denomination
  virtual int getNoteValue() const = 0;
};