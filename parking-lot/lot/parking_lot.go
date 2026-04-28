package lot

import (
	"parking-lot/fare"
	"parking-lot/manager"
	"parking-lot/ticket"
	"parking-lot/vehicle"
	"time"
)

type ParkingLot struct {
	manager        *manager.ParkingManager
	fareCalculator *fare.FareCalculator
	nextTicketID   int
}

func NewParkingLot(
	manager *manager.ParkingManager,
	calculator *fare.FareCalculator,
) *ParkingLot {
	return &ParkingLot{
		manager:        manager,
		fareCalculator: calculator,
		nextTicketID:   1,
	}
}

func (p *ParkingLot) EnterVehicle(v vehicle.Vehicle) *ticket.Ticket {
	spot := p.manager.ParkVehicle(v)
	if spot == nil {
		return nil
	}
	t := ticket.NewTicket(
		p.nextTicketID,
		v,
		spot,
		time.Now(),
	)
	p.nextTicketID++
	return t
}

func (p *ParkingLot) LeaveVehicle(t *ticket.Ticket) float64 {
	t.CloseTicket()
	p.manager.UnparkVehicle(t.Vehicle)
	return p.fareCalculator.CalculateFare(t)
}
