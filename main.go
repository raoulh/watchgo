package main

import (
	"log"
	"os"
	"runtime"
	"time"

	"github.com/jawher/mow.cli"
	"github.com/tarm/serial"
)

var (
	serialPort *string
)

func main() {
	runtime.GOMAXPROCS(runtime.NumCPU())

	app := cli.App("watchgo", "Simple client for arduino based serial watchdog")

	app.Spec = "SERIAL_PORT"

	serialPort = app.StringArg("SERIAL_PORT", "", "Serial port to use")

	app.Action = func() {
		log.Println("Serial Watchdog. Using port", *serialPort)

		runWatchdog(*serialPort)
	}

	app.Run(os.Args)
}

func runWatchdog(port string) {
	c := &serial.Config{
		Name:     port,
		Baud:     115200,
		Size:     8,
		Parity:   serial.ParityNone,
		StopBits: 1,
	}

	s, err := serial.OpenPort(c)
	if err != nil {
		log.Fatal(err)
	}

	for {
		_, err := s.Write([]byte("P"))
		if err != nil {
			log.Fatal(err)
			break
		}

		time.Sleep(500 * time.Millisecond)
	}
}
