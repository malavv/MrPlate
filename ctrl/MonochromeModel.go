package ctrl

import (
	"image/color"
)

var (
	MonochromeModel color.Model = color.ModelFunc(monochromeModel)
)

const (
	// Represents the chosen discretization threshold at which we consider the color white.
	MonochromeLuminosityThreshold = uint8(0x7f)
)

// Monochrome represents a boolean color with two possible values White or Black.
// Implements Go's color.Color interface with RGBA() conversion
type Monochrome struct {
	Y bool
}

func (c Monochrome) RGBA() (r, g, b, a uint32) {
	if c.Y {
		return color.White.RGBA()
	}
	return color.Black.RGBA()
}

func monochromeModel(c color.Color) color.Color {
  if _, ok := c.(Monochrome); ok {
    return c
  }
	// Uses Gray luminosity conversion modeling (weights differently different color)
	if color.GrayModel.Convert(c).(color.Gray).Y >= MonochromeLuminosityThreshold {
		return Monochrome{true}
	}
	return Monochrome{false}
}