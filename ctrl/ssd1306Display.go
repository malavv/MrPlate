package ctrl

import (
	"gobot.io/x/gobot/platforms/firmata/client"

	"time"
	"image"
	"image/color"
)

const (
	LcdWidth  = 128
	LcdHeight = 32

	// Commands
	contrastCmd           = 0x81
	displayAllOnResumeCmd = 0xA4
	normalDisplayCmd      = 0xA6
	displayOffCmd         = 0xAE
	displayOnCmd          = 0xAF
	displayOffsetCmd      = 0xD3
	comPinsCmd            = 0xDA
	vComDetectCmd         = 0xDB
	displayClockDivCmd    = 0xD5
	preChargeCmd          = 0xD9
	setMultiplexCmd       = 0xA8
	startLineCmd          = 0x40
	memoryModeCmd         = 0x20
	columnAddr            = 0x21
	pageAddr              = 0x22
	comScanDec            = 0xC8
	segremap              = 0xA0
	chargePump            = 0x8D
	deactivateScrollCmd   = 0x2E
)

// The SSD1306Display from Adafruit
// It implements the image.Image, and draw.Image interface
type Ssd1306Display struct {
	// Digital Pin used to send the reset Commands.
	ResetPin int
	// Rect is the image's bounds.
	Rect image.Rectangle
	// Image's Pixel matching the internal data structure of the Ssd1306Display.
	Pix []uint8

	// Gobot Board Interface to communicate with the display.
	board *client.Client
	// Address of the LCD display on the I2C network.
	lcdAddress int
}

// Returns the color at the requested point
func (b *Ssd1306Display) At(x, y int) color.Color {
	if !(image.Point{x, y}.In(b.Rect)) {
		return Monochrome{}
	}
	return Monochrome{b.AtBool(x, y)}
}

// Returns the color at the requested point without the Color wrapping
func (b *Ssd1306Display) AtBool(x, y int) bool {
	if !(image.Point{x, y}.In(b.Rect)) {
		return false
	}
	return (b.Pix[b.byteIndex(x, y)] & byte(y & 7)) > 0
}

// Returns the bounds in which the image is defined.
func (b *Ssd1306Display) Bounds() image.Rectangle { return b.Rect }

// Clears the In-Memory Display Buffer.
func (b *Ssd1306Display) ClearDisplayBuffer() {
	b.Pix = make([]byte, LcdWidth * LcdHeight / 8)
}

// From the image.Image interface, returns the Monochrome ColorModel
func (b *Ssd1306Display) ColorModel() color.Model { return MonochromeModel }

// Connects to the display and initializes drawing primitives.
func (b *Ssd1306Display) Connect() error {
	// Setting reset Pin
	if err := b.board.SetPinMode(b.ResetPin, client.Output); err != nil {
		return err
	}
	b.InitDisplay()
	return nil
}

// Disconnects from the display and clears the screen.
func (b *Ssd1306Display) Disconnect() error {
	b.ClearDisplayBuffer()
	b.Display()
	return b.board.Disconnect()
}

// Sends the current Pixel buffer to screen through the I2C
// The display protocol of the SSD1306 requires packet that are 17 bytes wide. The first byte is the header 0x40
// then the next 16 bytes are 16 columns wide, and the 8 bits of the byte are 8 rows.
// |(0,0)| |(1,0)| ...
// | ... | | ... | ...
// |(0,7)| |(1,7)| ...
func (b *Ssd1306Display) Display() {
	b.cmd(columnAddr)
	b.cmd(0)
	b.cmd(LcdWidth - 1)

	b.cmd(pageAddr)
	b.cmd(0)
	b.cmd(3)

	// SSD1306 display requires packet that are 16 bytes wide

	bytesPerRow := b.Rect.Dx() / 8 // 1 byte per 8 pixels
	payload := make([]byte, bytesPerRow + 1)

	n := len(b.Pix) / 16 // per 16 bytes Packets
	for i := 0; i < n; i++ {
		payload[0] = 0x40
		copy(payload[1:17], b.Pix[i * 16:(i + 1) * 16])
		b.board.I2cWrite(b.lcdAddress, payload)
	}
}

// Display initialization function directly taken from Adafruit's code.
func (b *Ssd1306Display) InitDisplay() {
	// Reset Display Chip
	b.board.DigitalWrite(b.ResetPin, 1)
	<-time.After(1 * time.Millisecond)
	b.board.DigitalWrite(b.ResetPin, 0)
	<-time.After(10 * time.Millisecond)
	b.board.DigitalWrite(b.ResetPin, 1)

	// Display Initialization Sequence
	b.cmd(displayOffCmd)
	b.cmd(displayClockDivCmd)
	b.cmd(0x80)

	b.cmd(setMultiplexCmd)
	b.cmd(LcdHeight - 1)

	b.cmd(displayOffsetCmd)
	b.cmd(0x0)
	b.cmd(startLineCmd | 0x0)
	b.cmd(chargePump)
	b.cmd(0x14)
	b.cmd(memoryModeCmd)
	b.cmd(0x00)
	b.cmd(segremap | 0x1)
	b.cmd(comScanDec)
	b.cmd(comPinsCmd)
	b.cmd(0x02)
	b.cmd(contrastCmd)
	b.cmd(0x8F)
	b.cmd(preChargeCmd)
	b.cmd(0xF1)
	b.cmd(vComDetectCmd)
	b.cmd(0x40)
	b.cmd(displayAllOnResumeCmd)
	b.cmd(normalDisplayCmd)
	b.cmd(deactivateScrollCmd)
	b.cmd(displayOnCmd)
}

// Sets the color of a pixel. If not Monochrome, a conversion will be made.
func (b *Ssd1306Display) Set(x, y int, c color.Color) {
	if !(image.Point{x, y}.In(b.Rect)) {
		return
	}

	if MonochromeModel.Convert(c).(Monochrome).Y {
		b.Pix[b.byteIndex(x, y)] |= uint8(1 << uint8(y & 7))
	} else {
		b.Pix[b.byteIndex(x, y)] &= ^uint8(1 << uint8(y & 7))
	}
}

// Creates a new SSD1306 Display.
func NewSsd1306Display(resetPin int, LcdAddress int, board *client.Client) *Ssd1306Display {
	return &Ssd1306Display{
		ResetPin: resetPin,
		board:    board,

		lcdAddress: LcdAddress,
		Rect:       image.Rect(0, 0, LcdWidth, LcdHeight),
		// See At and Set to see the structure of the SSD 1306
		Pix: make([]byte, LcdWidth * LcdHeight / 8),
	}
}

// See Display as to get an explanation.
func (b *Ssd1306Display) byteIndex(x, y int) int {
	return x + (y / 8) * b.Rect.Dx()
}

// Send a command down the I2C.
func (b *Ssd1306Display) cmd(data byte) {
	b.board.I2cWrite(b.lcdAddress, []byte{byte(0x00), data})
}