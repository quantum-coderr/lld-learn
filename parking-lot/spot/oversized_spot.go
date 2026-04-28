package spot

import "parking-lot/vehicle"

type OversizedSpot struct {
	spotNumber int
	vehicle    vehicle.Vehicle
}

var _ ParkingSpot = (*OversizedSpot)(nil)

func NewOversizedSpot(spotNumber int) ParkingSpot {
	return &OversizedSpot{
		spotNumber: spotNumber,
		vehicle:    nil,
	}
}

func (o *OversizedSpot) IsAvailable() bool {
	return o.vehicle == nil
}

func (o *OversizedSpot) Occupy(vehicle vehicle.Vehicle) {
	o.vehicle = vehicle
}

func (o *OversizedSpot) Vacate() {
	o.vehicle = nil
}

func (o *OversizedSpot) GetSpotNumber() int {
	return o.spotNumber
}

func (o *OversizedSpot) GetSize() vehicle.VehicleSize {
	return vehicle.LARGE
}
