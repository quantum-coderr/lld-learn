package vehicle

type Truck struct {
	licensePlate string
}

func (t *Truck) GetLicensePlate() string {
	return t.licensePlate
}

func (t *Truck) GetSize() VehicleSize {
	return LARGE
}

func NewTruck(licensePlate string) Vehicle {
	return &Truck{licensePlate: licensePlate}
}
