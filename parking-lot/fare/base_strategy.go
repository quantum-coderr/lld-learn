package fare

import (
	"parking-lot/ticket"
	"parking-lot/vehicle"
)

type BaseFareStrategy struct{}

const (
	SMALL_RATE  = 1.0
	MEDIUM_RATE = 2.0
	LARGE_RATE  = 3.0
)

func (b *BaseFareStrategy) CalculateFare(
	t *ticket.Ticket,
	currentFare float64,
) float64 {

	duration := t.GetDuration().Minutes()

	var rate float64

	switch t.Vehicle.GetSize() {

	case vehicle.MEDIUM:
		rate = MEDIUM_RATE

	case vehicle.LARGE:
		rate = LARGE_RATE

	default:
		rate = SMALL_RATE
	}

	return currentFare + rate*duration
}
