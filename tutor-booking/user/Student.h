#pragma once
#include "../logger/Logger.h"
#include "User.h"
#include <string>
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// Student  –  extends User (Liskov Substitution Principle)
// ─────────────────────────────────────────────────────────────────────────────
class Student : public User {
public:
  Student(int id, string name) : User(id, name) {
    LOG.info("Student", "Registered student: " + name);
  }
};