#include <SPI.h>
#include "VoltageController.h"
#include "gamepadController.h"

GamepadController gamepad;

// --- MCP4231 
const byte ADDR_P0 = 0b00000000;
const byte ADDR_P1 = 0b00010000;

// MCP 0
const int CS0 = 10;
#define POT0_IN A0
#define POT1_IN A1

// MCP #1
const int SCK2  = 8;
const int MOSI2 = 7;
const int MISO2 = 6;
const int CS2   = 9;
#define POT2_IN A2
#define POT3_IN A3

int wiperPositionWhite = 252;                         // start 4.8V
int wiperPositionBlue = 132;                         // start 2.6
int wiperPositionPurple = 255 - wiperPositionBlue;  // start 2.4
int wiperPositionGrey = 126;                       // start 2.4 V
 
int wheelPosition = 0;

// jaki step na 1 pomiar
const int step = 1;
const int deadzone = 15;  

byte softTransfer(byte data) {
  byte inb = 0;
  for (int i = 7; i >= 0; i--) {
    digitalWrite(MOSI2, (data >> i) & 1);
    digitalWrite(SCK2, HIGH);
    inb |= (digitalRead(MISO2) << i);
    digitalWrite(SCK2, LOW);
  }
  return inb;
}
void writePotSoft(int csPin, byte addr, byte val) {
  digitalWrite(csPin, LOW);
  softTransfer(addr);
  softTransfer(val);
  digitalWrite(csPin, HIGH);
}
void writePotHard(int csPin, byte addr, byte val) {
  digitalWrite(csPin, LOW);
  SPI.transfer(addr);
  SPI.transfer(val);
  digitalWrite(csPin, HIGH);
}

void setup() {
  Serial.begin(115200);
  gamepad.setup();

  pinMode(CS0, OUTPUT); digitalWrite(CS0, HIGH);
  SPI.begin();

  pinMode(CS2, OUTPUT);  digitalWrite(CS2, HIGH);
  pinMode(SCK2, OUTPUT); digitalWrite(SCK2, LOW);
  pinMode(MOSI2, OUTPUT);
  pinMode(MISO2, INPUT);
}

void loop() {
  gamepad.update();
  int x = (int)gamepad.getRightX();   // od 0 do 255

  int center = 128;

  // SKRECANIE W PRAWO
  if (x > center + deadzone) {
  
    wiperPositionBlue =  153; //hyc na 3V 
    wiperPositionPurple = 255 - wiperPositionBlue;  //hyc na 2V  
    wheelPosition += step;

  }
  // SKRECANIE W LEWO
  else if (x < center - deadzone) {
   
    wiperPositionBlue =  102;   //hyc na 2V
    wiperPositionPurple = 255 - wiperPositionBlue; //hyc na 3V 
    wheelPosition -= step;

  } else {
    wiperPositionBlue = 132;
    wiperPositionPurple = 255 - wiperPositionBlue;
  }
  
  if (wheelPosition > 255)
      wheelPosition -= 256;
  else if(wheelPosition < 0)
      wheelPosition += 256;

  wiperPositionWhite = (252 * (abs(wheelPosition - 128))) / 128;
  wiperPositionGrey = (252* abs(128 - abs(wheelPosition - 64))) /128;

  writePotHard(CS0, ADDR_P0, wiperPositionBlue);
  writePotHard(CS0, ADDR_P1, wiperPositionPurple);
  writePotSoft(CS2, ADDR_P0, wiperPositionGrey);
  writePotSoft(CS2, ADDR_P1, wiperPositionWhite);

  delay(1000 / 30);
}
