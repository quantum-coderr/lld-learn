package ticket

import (
	"parking-lot/spot"
	"parking-lot/vehicle"
	"time"
)

type Ticket struct {
	Vehicle   vehicle.Vehicle
	ID        int
	Spot      spot.ParkingSpot
	EntryTime time.Time
	ExitTime  time.Time
	Fee       float64
}

func NewTicket(
	id int,
	vehicle vehicle.Vehicle,
	spot spot.ParkingSpot,
	entryTime time.Time,
) *Ticket {
	return &Ticket{
		ID:        id,
		Vehicle:   vehicle,
		Spot:      spot,
		EntryTime: entryTime,
	}
}

func (t *Ticket) CloseTicket() {
	t.ExitTime = time.Now()
}

func (t *Ticket) GetDuration() time.Duration {
	if t.ExitTime.IsZero() {
		return time.Since(t.EntryTime)
	}

	return t.ExitTime.Sub(t.EntryTime)
}
