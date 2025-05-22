#ifndef VOLTAGECONTROLLER_H
#define VOLTAGECONTROLLER_H
#include <Arduino.h>
#include <SPI.h>

// step for one measurement
const int step = 1;
const int deadzone = 15;

// MCP4231 

const byte ADDR_P0 = 0b00000000;
const byte ADDR_P1 = 0b00010000;

// MCP 0 - uses hardware SPI- standard SPI library
const int CS0 = 10;
#define POT0_IN A0
#define POT1_IN A1

// MCP 1 - second MCP device but not supported by hardware SPI
const int SCK2  = 8;
const int MOSI2 = 7;
const int MISO2 = 6;
const int CS2   = 9;
#define POT2_IN A2
#define POT3_IN A3

const int joystickCenterPosition = 128;

class VoltageController {
    
public:
    VoltageController();
    void setup(int wiperPositionWhite, int wiperPositionBlue, int wiperPositionGrey);
    void update(int joystickPosition);  
    byte softTransfer(byte data);                           // Software SPI transfer (bit-banging)
    void writePotSoft(int csPin, byte addr, byte val);      // writes to MCP using software SPI
    void writePotHard(int csPin, byte addr, byte val);      // writes to MCP using hardware SPI

private:
    int wiperPositionWhite;
    int wiperPositionBlue;
    int wiperPositionPurple;
    int wiperPositionGrey;                      
    int wheelPosition;
};

#endif // VOLTAGECONTROLLER_H
