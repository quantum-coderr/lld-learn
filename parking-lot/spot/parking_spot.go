package spot

import "parking-lot/vehicle"

type ParkingSpot interface {
	IsAvailable() bool
	Occupy(vehicle vehicle.Vehicle)
	Vacate()
	GetSpotNumber() int
	GetSize() vehicle.VehicleSize
}
