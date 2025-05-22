#include <SPI.h>
#include "voltageController.h"
#include "gamepadController.h"

GamepadController gamepad;
VoltageController voltageController;

void setup() {

  Serial.begin(115200);
  gamepad.setup();
  voltageController.setup(252, 132, 126);   //start position (white, blue, grey)

}

void loop() {

  gamepad.update();
  int joystickPosition = (int)gamepad.getRightX();   // position 0 - 255
  voltageController.update(joystickPosition);
  delay(1000 / 30);
  
}
