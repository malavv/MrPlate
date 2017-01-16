package main

import (
	"github.com/malavv/mr-plate/ctrl"

	"time"
	"fmt"

	"gobot.io/x/gobot/platforms/firmata/client"
	"github.com/tarm/serial"
	"log"
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
		Display: ctrl.NewSsd1306Display(comPort, PinDisplayReset, LcdAddress, board),
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

	fmt.Println("Starting Display Sequence.....")
	p.Display.Display()

	<- time.After(1 * time.Second)
	fmt.Println("Clearing Sequence.....")
	p.Display.ClearDisplayBuffer()
	p.Display.Display()

	fmt.Println("Test Sequence.....")
	<- time.After(1 * time.Second)
	p.Display.Test()
	p.Display.Display()
	return nil
}

func (p *MrPlate) Disconnect() {
  p.Display.Disconnect()
}