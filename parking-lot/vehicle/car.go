package vehicle

type Car struct {
	licensePlate string
}

func (c *Car) GetLicensePlate() string {
	return c.licensePlate
}

func (c *Car) GetSize() VehicleSize {
	return MEDIUM
}

func NewCar(licensePlate string) Vehicle {
	return &Car{licensePlate: licensePlate}
}
