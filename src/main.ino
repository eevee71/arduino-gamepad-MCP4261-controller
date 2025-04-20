#include "gamepadController.h"

GamepadController controller;

void setup() {
    controller.setup();
}

void loop() {
    controller.update();
}