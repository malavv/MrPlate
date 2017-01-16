package ctrl

import (
	"gobot.io/x/gobot/platforms/firmata/client"
	"fmt"
	"time"
	"image"
	"image/color"
)

const (
	SSD1306_LCDWIDTH = 128
	SSD1306_LCDHEIGHT = 32

	SSD1306_SETCONTRAST = 0x81
	SSD1306_DISPLAYALLON_RESUME = 0xA4
	SSD1306_DISPLAYALLON = 0xA5
	SSD1306_NORMALDISPLAY = 0xA6
	SSD1306_INVERTDISPLAY = 0xA7
	SSD1306_DISPLAYOFF = 0xAE
	SSD1306_DISPLAYON = 0xAF

	SSD1306_SETDISPLAYOFFSET = 0xD3
	SSD1306_SETCOMPINS = 0xDA

	SSD1306_SETVCOMDETECT = 0xDB

	SSD1306_SETDISPLAYCLOCKDIV = 0xD5
	SSD1306_SETPRECHARGE = 0xD9

	SSD1306_SETMULTIPLEX = 0xA8

	SSD1306_SETLOWCOLUMN = 0x00
	SSD1306_SETHIGHCOLUMN = 0x10

	SSD1306_SETSTARTLINE = 0x40

	SSD1306_MEMORYMODE = 0x20
	SSD1306_COLUMNADDR = 0x21
	SSD1306_PAGEADDR   = 0x22

	SSD1306_COMSCANINC = 0xC0
	SSD1306_COMSCANDEC = 0xC8

	SSD1306_SEGREMAP = 0xA0

	SSD1306_CHARGEPUMP = 0x8D

	SSD1306_EXTERNALVCC = 0x1
	SSD1306_SWITCHCAPVCC = 0x2

	SSD1306_ACTIVATE_SCROLL = 0x2F
	SSD1306_DEACTIVATE_SCROLL = 0x2E
	SSD1306_SET_VERTICAL_SCROLL_AREA = 0xA3
	SSD1306_RIGHT_HORIZONTAL_SCROLL = 0x26
	SSD1306_LEFT_HORIZONTAL_SCROLL = 0x27
	SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL = 0x29
	SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL = 0x2A
)

type Ssd1306Display struct {
	resetPin   int
	ComPort    string
	board      *client.Client
	LcdAddress int
	Image      *MonochromaticImage
}

func NewSsd1306Display(comPort string, resetPin int, LcdAddress int, board *client.Client) *Ssd1306Display {
	return &Ssd1306Display{
		ComPort: comPort,
		resetPin: resetPin,
		board: board,

		LcdAddress: LcdAddress,
		Image: NewMonochromaticImage(image.Rect(0, 0, SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT)),
	}
}
func (b *Ssd1306Display) Connect() error {
	// Setting reset Pin
	if err := b.board.SetPinMode(b.resetPin, client.Output); err != nil {
		return err
	}
	b.InitDisplay()
	return nil
}
func (b *Ssd1306Display) Disconnect() error {
	b.ClearDisplayBuffer()
	b.Display()
	return b.board.Disconnect()
}
func (b *Ssd1306Display) PrintInfo() {
	fmt.Println("firmware name : ", b.board.FirmwareName)
	fmt.Println("firmata version : ", b.board.ProtocolVersion)
}
func (b *Ssd1306Display) InitDisplay() {
	// Reset Display Chip
	b.board.DigitalWrite(b.resetPin, 1)
	<-time.After(1 * time.Millisecond)
	b.board.DigitalWrite(b.resetPin, 0)
	<-time.After(10 * time.Millisecond)
	b.board.DigitalWrite(b.resetPin, 1)

	// Init Sequence
	b.cmd(SSD1306_DISPLAYOFF)
	b.cmd(SSD1306_SETDISPLAYCLOCKDIV)
	b.cmd(0x80)

	b.cmd(SSD1306_SETMULTIPLEX)
	b.cmd(SSD1306_LCDHEIGHT - 1)

	b.cmd(SSD1306_SETDISPLAYOFFSET)
	b.cmd(0x0)
	b.cmd(SSD1306_SETSTARTLINE | 0x0)
	b.cmd(SSD1306_CHARGEPUMP)
	b.cmd(0x14)
	b.cmd(SSD1306_MEMORYMODE)
	b.cmd(0x00)
	b.cmd(SSD1306_SEGREMAP | 0x1)
	b.cmd(SSD1306_COMSCANDEC)
	b.cmd(SSD1306_SETCOMPINS)
	b.cmd(0x02)
	b.cmd(SSD1306_SETCONTRAST)
	b.cmd(0x8F)
	b.cmd(SSD1306_SETPRECHARGE)
	b.cmd(0xF1)
	b.cmd(SSD1306_SETVCOMDETECT)
	b.cmd(0x40)
	b.cmd(SSD1306_DISPLAYALLON_RESUME)
	b.cmd(SSD1306_NORMALDISPLAY)
	b.cmd(SSD1306_DEACTIVATE_SCROLL)
	b.cmd(SSD1306_DISPLAYON)
}
func (b *Ssd1306Display) Test() {

	b.Image.Set(0, 0, color.White)
	b.Image.Set(2, 0, color.White)
	b.Image.Set(5, 0, color.White)
	b.Image.Set(10, 0, color.White)

	/*
	gc := draw2dimg.NewGraphicContext(b.Image)

	// Set some properties
	gc.SetFillColor(color.Black)
	gc.SetStrokeColor(color.White)
	gc.SetLineWidth(3)

	//gc.ArcTo(128 / 2, 32 / 2, 10, 10, 0, 2 * math.Pi)

	//draw2dkit.Circle(gc, 0, 0, 10)
	draw2dkit.RoundedRectangle(gc, 2, 2, 126, 30, 5, 5)
	gc.FillStroke()

	// Text
	gc.SetFontData(draw2d.FontData{Name: "luxi", Family: draw2d.FontFamilySans, Style: draw2d.FontStyleBold})
	// Set the fill text color to black
	gc.SetFillColor(image.White)
	gc.SetStrokeColor(image.White)
	gc.SetFontSize(10)
	// Display Hello World
	gc.FillStringAt("Hello World", 0, 20)
	*/
}

func getBytes(img *MonochromaticImage) []uint8 {
	all := make([]bool, img.Rect.Max.X * img.Rect.Max.Y)
	res := make([]byte, img.Rect.Max.X * img.Rect.Max.Y / 8)
  i := 0
	// 1st B    2nd B
	//  ____    _____
	// |(0,0)| |(1,0)|
	// | ... | | ... |
	// |(0,7)| |(1,7)|
	//  -----   -----
	for y := 0; y < img.Rect.Max.Y / 8; y++ {
		for x := 0; x < img.Rect.Max.X; x++ {
			all[i] = img.AtBool(x, y + 0)
			i++
			all[i] = img.AtBool(x, y + 1)
			i++
			all[i] = img.AtBool(x, y + 2)
			i++
			all[i] = img.AtBool(x, y + 3)
			i++
			all[i] = img.AtBool(x, y + 4)
			i++
			all[i] = img.AtBool(x, y + 5)
			i++
			all[i] = img.AtBool(x, y + 6)
			i++
			all[i] = img.AtBool(x, y + 7)
			i++
		}
	}
	Write(res, all)
	return res
}

func (b *Ssd1306Display) Display() {
	b.cmd(SSD1306_COLUMNADDR)
	b.cmd(0)
	b.cmd(SSD1306_LCDWIDTH - 1)

	b.cmd(SSD1306_PAGEADDR)
	b.cmd(0)
	b.cmd(3)

	r := b.Image.Rect
	bytesPerRow := r.Dx() / 8 // 1 byte per 8 pixels
	payload := make([]byte, bytesPerRow + 1)



	bytes := getBytes(b.Image)
  n := len(bytes) / 16
  for i := 0; i < n; i++ {
		payload[0] = 0x40
		copy(payload[1:17], bytes[i*16:(i+1)*16])
		b.board.I2cWrite(0x3C, payload)
	}
}

func (b *Ssd1306Display) ClearDisplayBuffer() {
	b.Image.Clear()
}

func (b *Ssd1306Display) cmd(data byte) {
	b.board.I2cWrite(0x3C, []byte{byte(0x00), data})
}

// Packs an array of boolean values into an array of bytes.
func Write(dst []uint8, src []bool) {
	_ = dst[(len(src) - 1) / 8]  // Pre-Test for Out-Of-Bounds
	for idx, data := range src {
		if data {
			dst[idx / 8] |= uint8(1 << uint8(idx & 7)) // equivalent to % 8
		} else {
			dst[idx / 8] &= ^uint8(1 << uint8(idx & 7))
		}
	}
}