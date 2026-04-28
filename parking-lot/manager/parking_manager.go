package manager

import (
	"parking-lot/spot"
	"parking-lot/vehicle"
)

type ParkingManager struct {
	availableSpots map[vehicle.VehicleSize][]spot.ParkingSpot
	vehicleToSpot  map[string]spot.ParkingSpot
}

func NewParkingManager(
	availableSpots map[vehicle.VehicleSize][]spot.ParkingSpot,
) *ParkingManager {
	return &ParkingManager{
		availableSpots: availableSpots,
		vehicleToSpot:  make(map[string]spot.ParkingSpot),
	}
}

func (pm *ParkingManager) findSpotForVehicle(v vehicle.Vehicle) spot.ParkingSpot {
	vehicleSize := v.GetSize()
	for size := vehicleSize; size <= vehicle.LARGE; size++ {
		spots := pm.availableSpots[size]
		for _, s := range spots {
			if s.IsAvailable() {
				return s
			}
		}
	}
	return nil
}

func (pm *ParkingManager) ParkVehicle(v vehicle.Vehicle) spot.ParkingSpot {
	spot := pm.findSpotForVehicle(v)
	if spot == nil {
		return nil
	}
	spot.Occupy(v)
	pm.vehicleToSpot[v.GetLicensePlate()] = spot
	spots := pm.availableSpots[spot.GetSize()]
	for i, s := range spots {
		if s == spot {
			pm.availableSpots[spot.GetSize()] =
				append(spots[:i], spots[i+1:]...)
			break
		}
	}
	return spot
}

func (pm *ParkingManager) UnparkVehicle(v vehicle.Vehicle) {
	spot, exists := pm.vehicleToSpot[v.GetLicensePlate()]
	if !exists {
		return
	}
	spot.Vacate()
	delete(pm.vehicleToSpot, v.GetLicensePlate())
	pm.availableSpots[spot.GetSize()] = append(pm.availableSpots[spot.GetSize()], spot)
}
