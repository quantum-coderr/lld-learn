# 🅿️ Parking Lot — Low Level Design (LLD)

A clean, well-structured implementation of a **Parking Lot System** in Go, built as a Low Level Design (LLD) exercise. This project demonstrates key OOP principles and Gang of Four (GoF) design patterns in a practical, interview-ready context.

---

## 📁 Project Structure

```
parking-lot/
├── main.go            # Entry point — wires everything together and runs a demo
├── vehicle/           # Vehicle domain (interface + concrete types)
│   ├── vehicle.go     # Vehicle interface & VehicleSize enum
│   ├── car.go         # Car → MEDIUM size
│   ├── bike.go        # Bike → SMALL size
│   └── truck.go       # Truck → LARGE size
├── spot/              # Parking spot domain (interface + concrete types)
│   ├── parking_spot.go   # ParkingSpot interface
│   ├── compact_spot.go   # CompactSpot → fits SMALL vehicles
│   ├── regular_spot.go   # RegularSpot → fits MEDIUM vehicles
│   └── oversized_spot.go # OversizedSpot → fits LARGE vehicles
├── ticket/            # Ticket model — tracks parking session
│   └── ticket.go
├── fare/              # Fare calculation — Strategy pattern
│   ├── strategy.go        # FareStrategy interface
│   ├── base_strategy.go   # Rate by vehicle size × duration
│   ├── peak_strategy.go   # Peak-hour multiplier (1.5×)
│   └── calculator.go      # FareCalculator — chains strategies
├── manager/           # Spot allocation and tracking
│   └── parking_manager.go
└── lot/               # High-level Facade — entry & exit operations
    └── parking_lot.go
```

---

## 🚀 How to Run

```bash
cd parking-lot
go run main.go
```

**Sample Output:**
```
Car parked at spot: 3
Motorcycle parked at spot: 1
Truck parked at spot: 5
Vehicles parked... waiting 5 seconds
Car Fare: 10.166...
Motorcycle Fare: 5.083...
Truck Fare: 15.25...

Parking session completed
```

---

## 🧠 Design Patterns Used

### 1. Strategy Pattern — `fare/`

The fare calculation is decoupled into interchangeable strategies that are applied in a **chain (pipeline)**. Each strategy receives the current fare and can modify it.

```
FareStrategy (interface)
    ├── BaseFareStrategy   → base rate × duration, scaled by vehicle size
    └── PeakHoursFareStrategy → multiplies fare by 1.5× during rush hours (7–10am, 4–7pm)
```

**`FareCalculator`** iterates over all configured strategies, passing the running fare from one to the next:

```go
// fare/calculator.go
func (f *FareCalculator) CalculateFare(t *ticket.Ticket) float64 {
    fare := 5.0  // base entry fee
    for _, strategy := range f.strategies {
        fare = strategy.CalculateFare(t, fare)
    }
    return fare
}
```

> ✅ **Why it matters:** New pricing rules (weekends, discounts, membership) can be added as new `FareStrategy` implementations without touching existing code.

---

### 2. Facade Pattern — `lot/ParkingLot`

`ParkingLot` acts as a **single, simple interface** for all parking operations. Callers only need to call `EnterVehicle` and `LeaveVehicle` — the internal complexity (spot allocation, ticketing, fare calculation) is completely hidden.

```go
// lot/parking_lot.go
func (p *ParkingLot) EnterVehicle(v vehicle.Vehicle) *ticket.Ticket { ... }
func (p *ParkingLot) LeaveVehicle(t *ticket.Ticket) float64         { ... }
```

> ✅ **Why it matters:** Clients (`main.go`) don't need to know about `ParkingManager`, `FareCalculator`, or `Ticket` creation. Complexity is buried behind a clean surface.

---

## 🔑 SOLID Principles in Practice

### Single Responsibility Principle (SRP)

Each package and struct has exactly **one reason to change**:

| Component | Responsibility |
|---|---|
| `vehicle` | Defines vehicle types and sizes |
| `spot` | Defines parking spot types and availability |
| `ticket` | Tracks a single parking session (entry, exit, duration) |
| `manager` | Allocates and deallocates spots |
| `fare` | Calculates fees based on configurable strategies |
| `lot` | Orchestrates a vehicle's full parking lifecycle |

### Open/Closed Principle (OCP)

- **Vehicle types** — add a new vehicle (e.g., `Van`) by creating a new struct implementing `Vehicle`. No existing code changes.
- **Spot types** — add a `MotorcycleSpot` by implementing `ParkingSpot`. No changes needed elsewhere.
- **Fare strategies** — add `WeekendFareStrategy` or `MemberDiscountStrategy` by implementing `FareStrategy` and registering it in `main.go`.

### Liskov Substitution Principle (LSP)

Every concrete type is **interchangeable** with its interface:

- `*Car`, `*Bike`, `*Truck` → all satisfy `vehicle.Vehicle`
- `*CompactSpot`, `*RegularSpot`, `*OversizedSpot` → all satisfy `spot.ParkingSpot`
- `*BaseFareStrategy`, `*PeakHoursFareStrategy` → all satisfy `fare.FareStrategy`

Go enforces this with compile-time checks:

```go
// spot/compact_spot.go
var _ ParkingSpot = (*CompactSpot)(nil)  // compile-time interface check
```

### Interface Segregation Principle (ISP)

Interfaces are kept **small and focused**:

```go
// vehicle/vehicle.go — only what a vehicle needs to expose
type Vehicle interface {
    GetLicensePlate() string
    GetSize() VehicleSize
}

// spot/parking_spot.go — only what a spot needs to expose
type ParkingSpot interface {
    IsAvailable() bool
    Occupy(vehicle vehicle.Vehicle)
    Vacate()
    GetSpotNumber() int
    GetSize() vehicle.VehicleSize
}
```

### Dependency Inversion Principle (DIP)

High-level modules depend on **abstractions, not concretions**:

- `ParkingManager` depends on the `vehicle.Vehicle` and `spot.ParkingSpot` interfaces — not on `*Car` or `*CompactSpot`.
- `FareCalculator` depends on the `FareStrategy` interface — not on specific strategy structs.
- Concrete dependencies are **injected** at startup (`main.go`) rather than created inside the components.

---

## 🗺️ Component Interaction

```
main.go
  │
  ├── Creates spots (CompactSpot, RegularSpot, OversizedSpot)
  ├── Creates ParkingManager (receives spots map)
  ├── Creates FareCalculator (receives []FareStrategy)
  └── Creates ParkingLot (Facade — receives Manager + Calculator)
          │
          ├── EnterVehicle(v Vehicle)
          │       ├── ParkingManager.ParkVehicle(v)  → finds & occupies a spot
          │       └── ticket.NewTicket(...)           → issues a timestamped ticket
          │
          └── LeaveVehicle(t *Ticket)
                  ├── ticket.CloseTicket()            → stamps exit time
                  ├── ParkingManager.UnparkVehicle(v) → frees the spot
                  └── FareCalculator.CalculateFare(t) → runs strategy chain
```

---

## 🧩 Key Design Decisions

### Smart Spot Allocation (Upward Fit)

`ParkingManager.findSpotForVehicle` searches from the vehicle's own size upwards — a SMALL vehicle can be placed in a MEDIUM or LARGE spot if compact spots are full:

```go
for size := vehicleSize; size <= vehicle.LARGE; size++ {
    // find any available spot at this size or larger
}
```

### Fare Calculation Pipeline

The `FareCalculator` uses a sequential pipeline of strategies. The **order matters** — `BaseFareStrategy` must run before `PeakHoursFareStrategy` since the peak multiplier applies to the base rate:

```
Base (₹1–3 × minutes) → Peak (×1.5 if rush hour) → Final Fare
```

### Ticket as Session Record

`Ticket` acts as the **single source of truth** for a parking session. Both the manager (to identify the vehicle's spot) and the fare calculator (to compute duration and size) work through the same ticket object.

---

## 📐 Class Diagram (Simplified)

```
Vehicle (interface)
  ├── Car    (MEDIUM)
  ├── Bike   (SMALL)
  └── Truck  (LARGE)

ParkingSpot (interface)
  ├── CompactSpot   (SMALL)
  ├── RegularSpot   (MEDIUM)
  └── OversizedSpot (LARGE)

FareStrategy (interface)
  ├── BaseFareStrategy
  └── PeakHoursFareStrategy

Ticket
  ├── Vehicle
  ├── ParkingSpot
  ├── EntryTime / ExitTime
  └── GetDuration()

ParkingManager
  ├── availableSpots map[VehicleSize][]ParkingSpot
  ├── ParkVehicle(Vehicle) ParkingSpot
  └── UnparkVehicle(Vehicle)

FareCalculator
  ├── strategies []FareStrategy
  └── CalculateFare(Ticket) float64

ParkingLot  ← Facade
  ├── manager        *ParkingManager
  ├── fareCalculator *FareCalculator
  ├── EnterVehicle(Vehicle) *Ticket
  └── LeaveVehicle(*Ticket) float64
```

---

## 🔭 Possible Extensions

| Feature | How to Extend |
|---|---|
| New vehicle type (e.g. `Van`) | Implement `Vehicle` interface |
| New spot type (e.g. `HandicappedSpot`) | Implement `ParkingSpot` interface |
| New pricing rule (e.g. membership discount) | Implement `FareStrategy`, inject in `main.go` |
| Multiple floors | Add a `floor` field to spots; `ParkingManager` groups by floor |
| Persistent storage | Swap `ParkingManager`'s in-memory maps for a DB-backed repository |
| Entry/exit gates | Wrap `ParkingLot.EnterVehicle` / `LeaveVehicle` behind gate handlers |

---

## 📚 Concepts to Review Alongside This Project

- **Design Patterns**: Strategy, Facade, (can extend with Factory, Observer)
- **SOLID Principles**: All five are demonstrated above
- **Go Interfaces**: Implicit satisfaction, compile-time assertions (`var _ I = (*T)(nil)`)
- **Dependency Injection**: Constructor injection used throughout

---

*Part of the [`lld-learn`](../) repository — a collection of Low Level Design problems implemented.*
