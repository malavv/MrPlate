package main

import (
	"github.com/malavv/mr-plate/ctrl"

	"time"
	"fmt"

	"gobot.io/x/gobot/platforms/firmata/client"
	"github.com/tarm/serial"
	"log"
	"image/color"
	"image"
	"image/draw"
)

const (
	ComPort = "COM4"
)

const (
	PinDisplayReset = 16
	BaudRate = 57600
	LcdAddress = 0x3C

	//PinSignalLed = "13"
	//PinButton1 = "14"
	//PinButton2 = "15"

	//PinPwm = "20"
)

func main() {
	plate := NewMrPlate(ComPort)

	if err := plate.InitAndConnect(); err != nil {
		log.Fatal(err)
	}

	fmt.Println("Test Sequence.....")

	// Looking into
  //   func NewGraphicContextWithPainter(img draw.Image, painter Painter) *GraphicContext {

	// Check also
	// NewMonochromePainter creates a new MonochromePainter that wraps the given
	// Painter.
	//
	// func NewMonochromePainter(p Painter) *MonochromePainter {
	//   return &MonochromePainter{Painter: p}
  // }

	wh := &image.Uniform{C: color.White}




	ctrl.Rect(plate.Display, image.Rect(1, 1,63, 15), 3, wh, image.ZP, draw.Src)
	ctrl.Rect(plate.Display, image.Rect(65, 1,126, 15), 3, wh, image.ZP, draw.Src)

	for i := 0; i < 128; i += 4 {
		plate.Display.Set(i, 25, color.White)
	}

	plate.Display.Display()

	<- time.After(10 * time.Second)
	defer plate.Disconnect()
}

type MrPlate struct {
	ComPort string
	Display *ctrl.Ssd1306Display
	Board *client.Client
}

func NewMrPlate(comPort string) *MrPlate {
	board := client.New()
	return &MrPlate{
		ComPort: comPort,
		Display: ctrl.NewSsd1306Display(PinDisplayReset, LcdAddress, board),
		Board: board,
	}
}

func (p *MrPlate) InitAndConnect() error {
	// Opening Serial Connection
	sp, err := serial.OpenPort(&serial.Config{ Name: p.ComPort, Baud: BaudRate })
	if err != nil {
		return err
	}
	// Creating a board
	if err = p.Board.Connect(sp); err != nil {
		return err
	}
	// Connecting I2C
	p.Board.I2cConfig(0)
	<- time.After(50 * time.Millisecond)

	fmt.Println("Connecting ...")
	if err := p.Display.Connect(); err != nil {
		log.Println("Error while connecting to the Display")
		return err
	}

	fmt.Println("Clearing Sequence.....")
	p.Display.ClearDisplayBuffer()
	p.Display.Display()

	return nil
}
func (p *MrPlate) Disconnect() {
  p.Display.Disconnect()
}
func (p *MrPlate) PrintInfo() {
	fmt.Println("firmware name : ", p.Board.FirmwareName)
	fmt.Println("firmata version : ", p.Board.ProtocolVersion)
}

/*
func (p *Ssd1306Display) Test() {

	p.Set(0, 0, color.White)
	p.Set(2, 0, color.White)
	p.Set(5, 0, color.White)
	p.Set(10, 0, color.White)


	gc := draw2dimg.NewGraphicContext(p.Image)

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

}
*/
