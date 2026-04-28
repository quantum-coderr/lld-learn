package main

import (
	"fmt"
	"parking-lot/fare"
	"parking-lot/lot"
	"parking-lot/manager"
	"parking-lot/spot"
	"parking-lot/vehicle"
	"time"
)

func main() {
	// Create Parking Spots
	compactSpots := []spot.ParkingSpot{
		spot.NewCompactSpot(1),
		spot.NewCompactSpot(2),
	}

	regularSpots := []spot.ParkingSpot{
		spot.NewRegularSpot(3),
		spot.NewRegularSpot(4),
	}

	largeSpots := []spot.ParkingSpot{
		spot.NewOversizedSpot(5),
	}

	availableSpots := map[vehicle.VehicleSize][]spot.ParkingSpot{
		vehicle.SMALL:  compactSpots,
		vehicle.MEDIUM: regularSpots,
		vehicle.LARGE:  largeSpots,
	}

	// Create Parking Manager
	parkingManager := manager.NewParkingManager(availableSpots)

	// Create Fare Calculator

	strategies := []fare.FareStrategy{
		&fare.BaseFareStrategy{},
		&fare.PeakHoursFareStrategy{},
	}

	fareCalculator := fare.NewFareCalculator(strategies)

	// Create Parking Lot (Facade)
	parkingLot := lot.NewParkingLot(parkingManager, fareCalculator)

	// Create Vehicles
	car := vehicle.NewCar("HR26AB1234")
	motorcycle := vehicle.NewBike("DL01XY1111")
	truck := vehicle.NewTruck("PB10TRUCK")

	// Vehicle Entry
	carTicket := parkingLot.EnterVehicle(car)
	fmt.Println("Car parked at spot:", carTicket.Spot.GetSpotNumber())

	motoTicket := parkingLot.EnterVehicle(motorcycle)
	fmt.Println("Motorcycle parked at spot:", motoTicket.Spot.GetSpotNumber())

	truckTicket := parkingLot.EnterVehicle(truck)
	fmt.Println("Truck parked at spot:", truckTicket.Spot.GetSpotNumber())

	// Simulate Parking Duration
	fmt.Println("Vehicles parked... waiting 5 seconds")
	time.Sleep(5 * time.Second)

	// Vehicle Exit
	carFare := parkingLot.LeaveVehicle(carTicket)
	fmt.Println("Car Fare:", carFare)

	motoFare := parkingLot.LeaveVehicle(motoTicket)
	fmt.Println("Motorcycle Fare:", motoFare)

	truckFare := parkingLot.LeaveVehicle(truckTicket)
	fmt.Println("Truck Fare:", truckFare)

	fmt.Println("\nParking session completed")
}
