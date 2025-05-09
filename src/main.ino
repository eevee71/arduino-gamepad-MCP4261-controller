#include <SPI.h>
#include "VoltageController.h"
#include "gamepadController.h"

GamepadController gamepad;

// --- MCP4231 setup ----------------------------------------------------
const byte ADDR_P0 = 0b00000000;
const byte ADDR_P1 = 0b00010000;

// hardware‑SPI (MCP #0)
const int CS0 = 10;
#define POT0_IN A0
#define POT1_IN A1

// soft‑SPI (bit‑bang) (MCP #1)
const int SCK2  = 8;
const int MOSI2 = 7;
const int MISO2 = 6;
const int CS2   = 9;
#define POT2_IN A2
#define POT3_IN A3

// aktualna pozycja wipera (0–255)
int wiperPosition = 128;

// ile kroków na pomiar (reguluje prędkość przesuwu)
const int stepSize = 1;

// deadzone wokół środka, w której joystick nie powoduje ruchu
const int deadzone = 15;  

// ----------------------------------------------------------------------
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
  int x = (int)gamepad.getRightX();   // 0…255

  // środek = 128
  int center = 128;

  // jeżeli powyżej deadzone w prawo → zwiększ
  if (x > center + deadzone) {
    wiperPosition = min(255, wiperPosition + stepSize);
  }
  // jeżeli poniżej deadzone w lewo → zmniejsz
  else if (x < center - deadzone) {
    wiperPosition = max(0, wiperPosition - stepSize);
  }

  writePotHard(CS0, ADDR_P0, wiperPosition);
  writePotHard(CS0, ADDR_P1, 255 - wiperPosition);
  writePotSoft(CS2, ADDR_P0, wiperPosition);
  writePotSoft(CS2, ADDR_P1, 255 - wiperPosition);

  Serial.print("JoyX="); Serial.print(x);
  Serial.print("  W="); Serial.print(wiperPosition);
  Serial.print("  A0="); Serial.print(analogRead(POT0_IN));
  Serial.print("  A1="); Serial.print(analogRead(POT1_IN));
  Serial.print("  A2="); Serial.print(analogRead(POT2_IN));
  Serial.print("  A3="); Serial.println(analogRead(POT3_IN));

  delay(1000 / 60);
}
