package ctrl

import (
	"image"
	"image/color"
)

var (
	MonochromeModel color.Model = color.ModelFunc(monochromeModel)
)

// Monochrome represents a boolean color, White or Black.
type Monochrome struct {
	Y bool
}

func (c Monochrome) RGBA() (r, g, b, a uint32) {
	var y uint32 = 0
	if c.Y {
		y = uint32(0xffff)
	}
	return y, y, y, y
}

func monochromeModel(c color.Color) color.Color {
  if _, ok := c.(Monochrome); ok {
    return c
  }
	gray := color.GrayModel.Convert(c).(color.Gray)
	if gray.Y > uint8(0x7f) {
		return Monochrome{true}
	}
	return Monochrome{false}
}

type MonochromaticImage struct {
	// Pix holds the image's pixels, as Monochrome values. The pixel at
	// (x, y) starts at Pix[(y-Rect.Min.Y)*Stride + (x-Rect.Min.X)*1].
	Pix []bool
	// Stride is the Pix stride (in bytes) between vertically adjacent pixels.
	Stride int
	// Rect is the image's bounds.
	Rect image.Rectangle
}

func (p *MonochromaticImage) ColorModel() color.Model { return MonochromeModel }
func (p *MonochromaticImage) Bounds() image.Rectangle { return p.Rect }
func (p *MonochromaticImage) At(x, y int) color.Color {
	if !(image.Point{x, y}.In(p.Rect)) {
		return Monochrome{}
	}
	i := p.PixOffset(x, y)
	return Monochrome{p.Pix[i]}
}
func (p *MonochromaticImage) AtBool(x, y int) bool {
	if !(image.Point{x, y}.In(p.Rect)) {
		return false
	}
	i := p.PixOffset(x, y)
	return p.Pix[i]
}

// PixOffset returns the index of the first element of Pix that corresponds to
// the pixel at (x, y).
func (p *MonochromaticImage) PixOffset(x, y int) int {
  return (y - p.Rect.Min.Y) * p.Stride + (x - p.Rect.Min.X)
}
func (p *MonochromaticImage) Clear() {
	p.Pix = make([]bool, 1 * p.Rect.Dx() * p.Rect.Dy())
}
func (p *MonochromaticImage) Set(x, y int, c color.Color) {
	if !(image.Point{x, y}.In(p.Rect)) {
		return
	}
	offset := p.PixOffset(x, y)
	value := MonochromeModel.Convert(c).(Monochrome).Y
	p.Pix[offset] = value
	//fmt.Println(p.At(5, 5))
}

func NewMonochromaticImage(r image.Rectangle) *MonochromaticImage {
	w, h := r.Dx(), r.Dy()
	pix := make([]bool, 1 * w * h)
	return &MonochromaticImage{pix, 1 * w, r}
}