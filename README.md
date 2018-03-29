# watchgo

This is a simple project for a hardware watchdog for a PC.

Two components are used:
 - An arduino plugged into USB and with a relay on an output pin connected to the RESET pins on the PC.
 - A small go service that ping the arduino.

After 3 minutes of inactivity (no ping message received), the arduino will trigger the relay and it resets the PC.

## Installation of the service
```
 # sudo cp watchgo@.service /etc/systemd/system
 # go get github.com/raoulh/watchgo
 # sudo cp $GOPATH/bin/watchgo /usr/bin
 # sudo systemctl daemon-reload
 # sudo systemctl enable watchgo@ttyUSB0
 # sudo systemctl start watchgo@ttyUSB0
```
