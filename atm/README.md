# ATM Cash Dispenser — Chain of Responsibility (C++)

A focused LLD implementation of the **Chain of Responsibility** design pattern applied to an ATM cash-dispensing problem.

---

## Design Pattern: Chain of Responsibility

> Avoid coupling the sender of a request to its receiver by giving more than one object a chance to handle the request.
> — GoF

Each denomination handler decides how many notes it can dispense, then **passes the remainder down the chain**.  
The chain is assembled once at startup and is completely transparent to the caller (`CashDispenser`).

---

## Architecture

```
CashDispenser          ← Public API / chain assembler
      │
      ▼
DispenseHandler        ← Abstract handler (interface)
      │
      ▼
NoteDispenser(2000)
      │  remainder
      ▼
NoteDispenser(500)
      │  remainder
      ▼
NoteDispenser(200)
      │  remainder
      ▼
NoteDispenser(100)
      │  remainder
      ▼
NoteDispenser(50)
      │  remainder
      ▼
NoteDispenser(20)      ← Last handler; throws if remainder ≠ 0
```

---

## File Structure

```
atm/
├── include/
│   ├── dispense_handler.h   # Abstract handler interface (DispenseHandler)
│   ├── note_dispenser.h     # Concrete handler (NoteDispenser)
│   └── cash_dispenser.h     # Public API + chain assembler
├── src/
│   ├── note_dispenser.cpp   # Dispense logic + chain forwarding
│   ├── cash_dispenser.cpp   # Builds the chain bottom-up; validates input
│   └── test_dispenser.cpp   # main() — integration tests
├── build/                   # Object files (git-ignored)
├── Makefile
└── README.md
```

---

## Class Responsibilities

| Class | Responsibility |
|---|---|
| `DispenseHandler` | Abstract base. Holds `unique_ptr<DispenseHandler> nextHandler`. Defines `dispense(int)` and `setNext()`. |
| `NoteDispenser` | Concrete handler. Knows its `noteValue`. Dispenses as many notes as possible, forwards remainder to next. |
| `CashDispenser` | Owns the chain head. Validates input (`> 0`, multiple of 10). Delegates to chain. |

---

## SOLID Principles Applied

| Principle | How |
|---|---|
| **S**ingle Responsibility | Each `NoteDispenser` handles exactly one denomination. `CashDispenser` only assembles and invokes the chain. |
| **O**pen/Closed | Add a new denomination by creating one more `NoteDispenser` and inserting it in `buildChain()` — zero changes to existing classes. |
| **L**iskov Substitution | Any `DispenseHandler*` can be placed in the chain; callers don't know the concrete type. |
| **D**ependency Inversion | `CashDispenser` depends on the `DispenseHandler` abstraction, not `NoteDispenser` directly. |

---

## Key Implementation Detail — Chain Built Bottom-Up

```cpp
// ✅ Correct: build tail → head (each ptr still valid when setNext is called)
dispenser50->setNext(std::move(dispenser20));
dispenser100->setNext(std::move(dispenser50));
dispenser200->setNext(std::move(dispenser100));
dispenser500->setNext(std::move(dispenser200));
dispenser2000->setNext(std::move(dispenser500));

// ❌ Wrong (use-after-move UB): dispenser500 is null after being moved into dispenser2000
dispenser2000->setNext(std::move(dispenser500));
dispenser500->setNext(std::move(dispenser200)); // segfault!
```

---

## Build & Run

```bash
make          # builds atm_test into build/
./atm_test    # runs integration tests
make clean    # removes build/ and binary
```

### Expected Output

```
=== Test 1: ₹2000 ===
=== Attempting to dispense ₹2000 ===
Dispensing 1 note(s) of ₹2000
=== Cash dispensed successfully ===

=== Test 2: ₹750 ===
=== Attempting to dispense ₹750 ===
Dispensing 1 note(s) of ₹500
Remaining ₹250 passed to next handler...
Dispensing 1 note(s) of ₹200
Remaining ₹50 passed to next handler...
Dispensing 1 note(s) of ₹50
=== Cash dispensed successfully ===

=== Test 3: ₹1230 ===
=== Attempting to dispense ₹1230 ===
Dispensing 1 note(s) of ₹1000
...
Error: Cannot dispense remaining amount: ₹30

=== Test 4: Invalid Amount ===
Error: Amount must be a multiple of 10
```

---

## Extending the Chain

To add a ₹10 denomination:

1. No new class needed — `NoteDispenser` is generic.
2. In `cash_dispenser.cpp::buildChain()`:

```cpp
auto dispenser10 = std::make_unique<NoteDispenser>(10);
dispenser20->setNext(std::move(dispenser10));   // append at tail
```

That's it. Zero changes to `DispenseHandler`, `NoteDispenser`, or `CashDispenser`.
