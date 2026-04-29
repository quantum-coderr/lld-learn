#pragma once
#include "../logger/Logger.h"
#include "Student.h"
#include "Tutor.h"
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// UserFactory  –  Factory Method Pattern
// Centralises object creation; clients never call `new` directly.
// Demonstrates: Single Responsibility + Open/Closed Principle
// ─────────────────────────────────────────────────────────────────────────────
class UserFactory {
public:
  static Student *createStudent(int id, string name) {
    LOG.info("UserFactory", "Creating student → id=" + to_string(id) + " name=" + name);
    return new Student(id, name);
  }

  static Tutor *createTutor(int id, string name, string s) {
    LOG.info("UserFactory", "Creating tutor → id=" + to_string(id) +
                                " name=" + name + " subject=" + s);
    return new Tutor(id, name, s);
  }
};