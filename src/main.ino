#include "VoltageController.h"
#include "gamepadController.h"

VoltageController voltCtrl(10, 8);  
GamepadController gamepad;

void setup() {

    voltCtrl.begin();
    gamepad.setup();
}

void loop() {

  gamepad.update();
    byte leftX = gamepad.getRightX();
    voltCtrl.update(leftX);

    delay(1000 / 60);  
}
