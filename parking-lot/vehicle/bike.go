package vehicle

type Bike struct {
	licensePlate string
}

func (b *Bike) GetLicensePlate() string {
	return b.licensePlate
}

func (b *Bike) GetSize() VehicleSize {
	return SMALL
}

func NewBike(licensePlate string) Vehicle {
	return &Bike{licensePlate: licensePlate}
}
