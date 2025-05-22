#include "voltageController.h"

VoltageController::VoltageController(){}

byte VoltageController::softTransfer(byte data) {
  byte inb = 0;
  for (int i = 7; i >= 0; i--) {
    digitalWrite(MOSI2, (data >> i) & 1);
    digitalWrite(SCK2, HIGH);
    inb |= (digitalRead(MISO2) << i);
    digitalWrite(SCK2, LOW);
  }
  return inb;
}
void VoltageController::writePotSoft(int csPin, byte addr, byte val) {
  digitalWrite(csPin, LOW);
  softTransfer(addr);
  softTransfer(val);
  digitalWrite(csPin, HIGH);
}
void VoltageController::writePotHard(int csPin, byte addr, byte val) {
  digitalWrite(csPin, LOW);
  SPI.transfer(addr);
  SPI.transfer(val);
  digitalWrite(csPin, HIGH);
}

void VoltageController::setup(int wiperPositionWhite, int wiperPositionBlue, int wiperPositionGrey) {

    this->wiperPositionWhite = wiperPositionWhite;          // start 4.8V
    this->wiperPositionBlue = wiperPositionBlue;           // start 2.6
    this->wiperPositionPurple = 255 - wiperPositionBlue;  // start 2.4
    this->wiperPositionGrey = wiperPositionGrey;         // start 2.4 V
    this->wheelPosition = 0;

    pinMode(CS0, OUTPUT); digitalWrite(CS0, HIGH);
    SPI.begin();
    pinMode(CS2, OUTPUT);  digitalWrite(CS2, HIGH);
    pinMode(SCK2, OUTPUT); digitalWrite(SCK2, LOW);
    pinMode(MOSI2, OUTPUT);
    pinMode(MISO2, INPUT);
}

void VoltageController::update(int joystickPosition) {

    // RIGHT TURN
  if (joystickPosition > joystickCenterPosition + deadzone) {
  
    wiperPositionBlue =  153;                               //jump to 3V 
    wiperPositionPurple = 255 - wiperPositionBlue;          //jump to 2V  
    wheelPosition += step;

  }

  // LEFT TURN
  else if (joystickPosition < joystickCenterPosition - deadzone) {
   
    wiperPositionBlue =  102;                                   //jump to 2V
    wiperPositionPurple = 255 - wiperPositionBlue;              //jump to 3V 
    wheelPosition -= step;

  } else {
    wiperPositionBlue = 132;
    wiperPositionPurple = 255 - wiperPositionBlue;
  }
  
  if (wheelPosition > 255)
      wheelPosition -= 256;
  else if(wheelPosition < 0)
      wheelPosition += 256;

  //white and grey - voltage function
  wiperPositionWhite = (252 * (abs(wheelPosition - 128))) / 128;           
  wiperPositionGrey = (252* abs(128 - abs(wheelPosition - 64))) /128;

  writePotHard(CS0, ADDR_P0, wiperPositionBlue);
  writePotHard(CS0, ADDR_P1, wiperPositionPurple);
  writePotSoft(CS2, ADDR_P0, wiperPositionGrey);
  writePotSoft(CS2, ADDR_P1, wiperPositionWhite);
}