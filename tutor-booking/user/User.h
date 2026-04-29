#pragma once
#include <string>
using namespace std;

class User {
protected:
  int id;
  string name;

public:
  User(int id, string name) : id(id), name(name) {}

  int getId() { return id; }
  string getName() { return name; }
};