package fare

import "parking-lot/ticket"

type FareStrategy interface {
	CalculateFare(t *ticket.Ticket, currentFare float64) float64
}
