package fare

import (
	"parking-lot/ticket"
)

type PeakHoursFareStrategy struct{}

const PEAK_MULTIPLIER = 1.5

func (p *PeakHoursFareStrategy) CalculateFare(
	t *ticket.Ticket,
	currentFare float64,
) float64 {
	hour := t.EntryTime.Hour()
	if (hour >= 7 && hour <= 10) ||
		(hour >= 16 && hour <= 19) {

		return currentFare * PEAK_MULTIPLIER
	}
	return currentFare
}
