#ifndef GAMEPAD_CONTROLLER_H
#define GAMEPAD_CONTROLLER_H

#include <Arduino.h>
#include <iMakerPS2.h>
#include <avr/pgmspace.h>

class GamepadController {

 private:
    void controller(byte data);
    void dumpButtons(PsxButtons psxButtons);
    void dumpAnalog(const char* str, byte x, byte y);
    void configureController();
    byte psxButtonToIndex(PsxButtons psxButtons);

    static const byte PIN_PS2_ATT = 4;
    static const byte PIN_PS2_CMD = 5;
    static const byte PIN_PS2_DAT = 6;
    static const byte PIN_PS2_CLK = 7;

    PsxControllerBitBang<PIN_PS2_ATT, PIN_PS2_CMD, PIN_PS2_DAT, PIN_PS2_CLK> psx;
    boolean haveController;
    byte slx, sly, srx, sry;
 public:
    GamepadController();
    void setup();
    void update();
    byte getRightX() const;
};

#endif