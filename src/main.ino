#include "VoltageController.h"
#include "gamepadController.h"

GamepadController gamepad;

void setup() {
    gamepad.setup();
}

void loop() {

  gamepad.update();
  byte rightX = gamepad.getRightX();
  delay(1000 / 60);  
}
