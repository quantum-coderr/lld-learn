package spot

import "parking-lot/vehicle"

type CompactSpot struct {
	spotNumber int
	vehicle    vehicle.Vehicle
}

var _ ParkingSpot = (*CompactSpot)(nil)

func NewCompactSpot(spotNumber int) ParkingSpot {
	return &CompactSpot{
		spotNumber: spotNumber,
		vehicle:    nil,
	}
}

func (c *CompactSpot) IsAvailable() bool {
	return c.vehicle == nil
}

func (c *CompactSpot) Occupy(vehicle vehicle.Vehicle) {
	c.vehicle = vehicle
}

func (c *CompactSpot) Vacate() {
	c.vehicle = nil
}

func (c *CompactSpot) GetSpotNumber() int {
	return c.spotNumber
}

func (c *CompactSpot) GetSize() vehicle.VehicleSize {
	return vehicle.SMALL
}
