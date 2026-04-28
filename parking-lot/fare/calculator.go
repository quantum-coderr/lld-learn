package fare

import "parking-lot/ticket"

type FareCalculator struct {
	strategies []FareStrategy
}

func NewFareCalculator(strategies []FareStrategy) *FareCalculator {
	return &FareCalculator{
		strategies: strategies,
	}
}

func (f *FareCalculator) CalculateFare(t *ticket.Ticket) float64 {
	fare := 5.0
	for _, strategy := range f.strategies {
		fare = strategy.CalculateFare(t, fare)
	}
	return fare
}
