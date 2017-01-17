package ctrl

import (
	"image/draw"
	"image"
)

func HLine(img draw.Image, p0 image.Point, width, thick int, src image.Image, sp image.Point, op draw.Op) {
	delta := (thick - 1) / 2
	draw.Draw(img, image.Rect(p0.X, p0.Y - delta, p0.X + width, p0.Y + 1 + delta), src, sp, op)
}
func VLine(img draw.Image, p0 image.Point, height, thick int, src image.Image, sp image.Point, op draw.Op) {
	delta := (thick - 1) / 2
	draw.Draw(img, image.Rect(p0.X - delta, p0.Y, p0.X + 1 + delta, p0.Y + height), src, sp, op)
}

// Thickness only odd number. Draws only border, for fill use draw.Draw
func Rect(img draw.Image, rect image.Rectangle, thick int, src image.Image, sp image.Point, op draw.Op) {
	if thick <= 0 {
		return
	}
	delta := (thick - 1) / 2
	// top
	HLine(img, rect.Min, rect.Dx(), thick, src, sp, op)
	// Bottom
	HLine(img, image.Point{rect.Min.X, rect.Max.Y}, rect.Dx(), thick, src, sp, op)
  // Left
	VLine(img, image.Point{rect.Min.X, rect.Min.Y - delta}, rect.Dy() + 1 + 2 * delta, thick, src, sp, op) // Not quite sure why the +1
	// Right
	VLine(img, image.Point{rect.Max.X, rect.Min.Y - delta}, rect.Dy() + 1 + 2 * delta, thick, src, sp, op)
}

