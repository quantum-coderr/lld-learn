package spot

import "parking-lot/vehicle"

type RegularSpot struct {
	spotNumber int
	vehicle    vehicle.Vehicle
}

var _ ParkingSpot = (*RegularSpot)(nil)

func NewRegularSpot(spotNumber int) ParkingSpot {
	return &RegularSpot{
		spotNumber: spotNumber,
		vehicle:    nil,
	}
}

func (r *RegularSpot) IsAvailable() bool {
	return r.vehicle == nil
}

func (r *RegularSpot) Occupy(vehicle vehicle.Vehicle) {
	r.vehicle = vehicle
}

func (r *RegularSpot) Vacate() {
	r.vehicle = nil
}

func (r *RegularSpot) GetSpotNumber() int {
	return r.spotNumber
}

func (r *RegularSpot) GetSize() vehicle.VehicleSize {
	return vehicle.MEDIUM
}
