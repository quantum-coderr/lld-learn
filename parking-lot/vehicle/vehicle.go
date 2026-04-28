package vehicle

type VehicleSize int

const (
	SMALL VehicleSize = iota
	MEDIUM
	LARGE
)

type Vehicle interface {
	GetLicensePlate() string
	GetSize() VehicleSize
}
