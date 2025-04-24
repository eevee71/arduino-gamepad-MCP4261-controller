#include "VoltageController.h"
#include "gamepadController.h"

GamepadController gamepad;

void setup() {
    gamepad.setup();
}

void loop() {

  gamepad.update();
  delay(1000 / 60);  
}
