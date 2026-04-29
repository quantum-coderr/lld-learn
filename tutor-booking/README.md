# 📚 Tutor Booking System — LLD Practice

A Low-Level Design (LLD) exercise implementing a **Tutor Booking System** in C++.
The project demonstrates core **SOLID principles** and common **Design Patterns**
through a clean, layered architecture.

---

## 🚀 Quick Start

```bash
# Compile
g++ main.cpp -o tutor_booking

# Run
./tutor_booking
```

### Sample Output

```
──────────────────────────────────────────────────
  Tutor Booking System — Demo
──────────────────────────────────────────────────

[INFO]   [main]  Setting up users and slots...
[INFO]   [UserFactory]  Creating student → id=1 name=Rohan
[INFO]   [Student]  Registered student: Rohan
[INFO]   [UserFactory]  Creating tutor → id=2 name=Yash subject=Math
[INFO]   [Tutor]  Registered tutor: Yash | Subject: Math
[DEBUG]  [TimeSlot]  Created slot #1 [10:00 – 11:00]
[INFO]   [Tutor]  Yash → added slot [10:00 – 11:00]
[INFO]   [main]  Attempting Booking #1...

──────────────────────────────────────────────────
  Booking Request — Rohan → Math
──────────────────────────────────────────────────

[INFO]   [SearchService]  Searching for tutors with subject: "Math"
[INFO]   [SearchService]  Found 1 tutor(s) for subject: "Math"
[INFO]   [TutorBookingSystem]  Selected tutor: Yash
[INFO]   [SlotManager]  Scanning slots for tutor: Yash
[  OK  ] [SlotManager]  Available slot found → [10:00 – 11:00]
[INFO]   [BookingManager]  Attempting booking → Student: Rohan | Tutor: Yash | Slot: [10:00 – 11:00]
[DEBUG]  [TimeSlot]  Attempting to book slot #1
[  OK  ] [TimeSlot]  Slot #1 [10:00 – 11:00] successfully booked
[  OK  ] [BookingManager]  Booking #1 created for Rohan with tutor Yash
[  OK  ] [TutorBookingSystem]  ✔ Booking #1 completed successfully!

[  OK  ] [main]  Result → Booking Successful ✔
[INFO]   [main]  Attempting Booking #2 (same slot, should fail)...
...
[ WARN ] [SlotManager]  No available slot found for tutor: Yash
[ERROR]  [TutorBookingSystem]  No available slot for tutor Yash. Aborting.
[ERROR]  [main]  Result → Booking Failed ✘
```

---

## 🗂️ Project Structure

```
tutor-booking/
├── main.cpp                        # Entry point & demo driver
├── logger/
│   └── Logger.h                    # Singleton logger (colored output)
├── user/
│   ├── User.h                      # Abstract base user
│   ├── Student.h                   # Student (extends User)
│   ├── Tutor.h                     # Tutor (extends User)
│   └── UserFactory.h               # Factory for creating users
├── slot/
│   └── TimeSlot.h                  # Bookable time window (thread-safe)
├── booking/
│   ├── Booking.h                   # Booking entity + status enum
│   └── BookingRepository.h         # In-memory booking store
├── manager/
│   ├── SlotManager.h               # Scans tutor slots for availability
│   └── BookingManager.h            # Creates / cancels bookings
├── search/
│   └── SearchService.h             # Filters tutors by subject
└── system/
    └── TutorBookingSystem.h        # Facade: orchestrates all subsystems
```

---

## 🏛️ SOLID Principles

### S — Single Responsibility Principle

> *A class should have only one reason to change.*

| Class | Responsibility |
|---|---|
| `SearchService` | Only searches/filters tutors by subject |
| `SlotManager` | Only finds available slots |
| `BookingManager` | Only creates and cancels bookings |
| `BookingRepository` | Only stores and retrieves bookings |
| `Logger` | Only handles logging output |

Each class has exactly **one job**. If the search algorithm changes, only `SearchService` changes. If persistence moves to a DB, only `BookingRepository` changes.

---

### O — Open/Closed Principle

> *Classes should be open for extension, closed for modification.*

- `User` is a base class. New user types (`Admin`, `Parent`) can be added by extending `User` without modifying existing code.
- `Logger` supports new `LogLevel` variants without touching any consumer code.
- New search strategies (e.g., search by rating, location) can be added by extending `SearchService` or using strategy injection.

---

### L — Liskov Substitution Principle

> *Subtypes must be substitutable for their base types.*

```
User
 ├── Student   ← can be used wherever User is expected
 └── Tutor     ← can be used wherever User is expected
```

`Student` and `Tutor` are substitutable for `User` everywhere. `TutorBookingSystem` works with `Student*` and `Tutor*` polymorphically.

---

### I — Interface Segregation Principle

> *No client should be forced to depend on methods it does not use.*

- `SearchService` exposes only `searchBySubject()` — the system doesn't receive a fat interface with unrelated methods.
- `SlotManager` exposes only `findAvailableSlot()`.
- `BookingRepository` is separated from `BookingManager` so consumers that only need to *read* bookings aren't coupled to booking *creation* logic.

---

### D — Dependency Inversion Principle

> *High-level modules should not depend on low-level modules.*

`TutorBookingSystem` (high-level orchestrator) composes `SearchService`, `SlotManager`, and `BookingManager` as value members. It works against their interfaces, not concrete storage or search mechanisms. Swapping an in-memory repository for a database only requires changing `BookingRepository`, not `TutorBookingSystem`.

---

## 🎨 Design Patterns

### 1. Facade Pattern — `TutorBookingSystem`

Provides a **single, simple entry point** (`bookTutor()`) that hides the complexity of three coordinated subsystems:

```
TutorBookingSystem::bookTutor()
   ├── SearchService::searchBySubject()
   ├── SlotManager::findAvailableSlot()
   └── BookingManager::createBooking()
```

Clients only call `system.bookTutor(...)` — they never interact with the subsystems directly.

---

### 2. Factory Method Pattern — `UserFactory`

Centralises object creation. The `main()` function never calls `new Student(...)` or `new Tutor(...)` directly:

```cpp
Student *s = UserFactory::createStudent(1, "Rohan");
Tutor   *t = UserFactory::createTutor(2, "Yash", "Math");
```

**Benefits:**
- Decouples construction logic from usage
- Easy to add validation, logging, or pooling inside the factory
- Open for extension: add `createAdmin()` without changing call sites

---

### 3. Repository Pattern — `BookingRepository`

`BookingRepository` abstracts the storage layer from the business logic. `BookingManager` never knows *how* bookings are stored:

```cpp
repository.addBooking(booking);      // persists
repository.getBookings();            // retrieves
```

Swap the in-memory `vector` for a SQL/NoSQL store by only modifying `BookingRepository`.

---

### 4. Singleton Pattern — `Logger`

The `Logger` class uses the **Meyer's Singleton** (thread-safe in C++11+):

```cpp
static Logger &getInstance() {
    static Logger instance; // constructed once, lazily
    return instance;
}
```

**Why Singleton for Logger?**
- Ensures a single, consistent output stream across all components
- No need to pass a logger instance through constructors
- Access globally via `LOG.info(...)` macro

---

### 5. Thread-Safe Resource Locking — `TimeSlot`

`TimeSlot::bookSlot()` uses `std::lock_guard<std::mutex>` to prevent double-booking under concurrent access:

```cpp
bool bookSlot() {
    std::lock_guard<std::mutex> lock(mtx);  // RAII lock
    if (booked) return false;
    booked = true;
    return true;
}
```

This is the **RAII (Resource Acquisition Is Initialization)** idiom — the mutex is automatically released when `lock` goes out of scope, even on exceptions.

---

## 🔄 Booking Flow

```
main()
  │
  ├─ UserFactory::createStudent()    → Student*
  ├─ UserFactory::createTutor()      → Tutor*
  ├─ new TimeSlot()                  → TimeSlot*
  ├─ tutor->addSlot(slot)
  │
  └─ TutorBookingSystem::bookTutor(student, tutors, "Math")
        │
        ├─ SearchService::searchBySubject(tutors, "Math")
        │      └─ returns matched Tutor*
        │
        ├─ SlotManager::findAvailableSlot(tutor)
        │      └─ returns first available TimeSlot*
        │
        └─ BookingManager::createBooking(student, tutor, slot)
               ├─ TimeSlot::bookSlot()      ← mutex-guarded
               ├─ new Booking(...)
               └─ BookingRepository::addBooking(booking)
```

---

## 📝 Logger

The `Logger` (located in `logger/Logger.h`) uses ANSI color codes to make the execution flow readable at a glance:

| Level | Color | Use case |
|---|---|---|
| `INFO` | Cyan | General flow steps |
| `SUCCESS` | Green | Successful operations |
| `WARNING` | Yellow | Expected failures (slot taken, etc.) |
| `ERROR` | Red | Hard failures / null returns |
| `DEBUG` | Magenta | Low-level internals (slot creation, mutex ops) |

```cpp
LOG.info("Component", "message");
LOG.success("Component", "message");
LOG.warn("Component", "message");
LOG.error("Component", "message");
LOG.debug("Component", "message");
LOG.separator("Section Title");
```

---

## 📌 Key LLD Takeaways

1. **Separate concerns** — every class does one thing well.
2. **Program to abstractions** — orchestrators depend on service contracts, not implementations.
3. **Factories hide construction complexity** — clients don't care about `new`.
4. **Repositories isolate storage** — business logic never touches raw storage details.
5. **Facades simplify subsystems** — external callers get a clean, unified API.
6. **Locks protect shared mutable state** — always use RAII guards, never raw lock/unlock.
