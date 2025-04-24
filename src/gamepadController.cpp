#include "gamepadController.h"

typedef const __FlashStringHelper* FlashStr;
typedef const byte* PGM_BYTES_P;
#define PSTR_TO_F(s) reinterpret_cast<const __FlashStringHelper*>(s)
#define DEAF 255
#define SELECT 0
#define START 3
#define UP 4
#define RIGHT 5
#define DOWN 6
#define LEFT 7
#define L2 8
#define R2 9
#define L1 10
#define R1 11
#define TRIANGLE 12
#define CIRCLE 13
#define CROSS 14
#define SQUARE 15

GamepadController::GamepadController() : haveController(false), slx(0), sly(0), srx(0), sry(0) {}

void GamepadController::setup() {
    Serial.begin(9600);
    Serial.println(F("Ready!"));
}

void GamepadController::update() {
    if (!haveController) {
        if (psx.begin()) {
            Serial.println(F("Controller found!"));
            delay(300);
            configureController();
            haveController = true;
        }
    } else {
        if (!psx.read()) {
            Serial.println(F("Controller lost :("));
            haveController = false;
        } else {
            dumpButtons(psx.getButtonWord());

            byte lx, ly;
            psx.getLeftAnalog(lx, ly);
            if (lx != slx || ly != sly) {
                dumpAnalog("Left", lx, ly);
                slx = lx;
                sly = ly;
            }

            byte rx, ry;
            psx.getRightAnalog(rx, ry);
            if (rx != srx || ry != sry) {
                dumpAnalog("Right", rx, ry);
                srx = rx;
                sry = ry;
            }
        }
    }
    delay(1000 / 60);
}

void GamepadController::configureController() {
    if (!psx.enterConfigMode()) {
        Serial.println(F("Cannot enter config mode"));
        return;
    }

    if (!psx.enableAnalogSticks()) Serial.println(F("Cannot enable analog sticks"));
    if (!psx.enableAnalogButtons()) Serial.println(F("Cannot enable analog buttons"));
    if (!psx.exitConfigMode()) Serial.println(F("Cannot exit config mode"));
}

void GamepadController::controller(byte data) {
    switch (data) {
        case SELECT: Serial.println(F("SELECT")); break;
        case START: Serial.println(F("START")); break;
        case UP: Serial.println(F("UP")); break;
        case RIGHT: Serial.println(F("RIGHT")); break;
        case DOWN: Serial.println(F("DOWN")); break;
        case LEFT: Serial.println(F("LEFT")); break;
        case L2: Serial.println(F("L2")); break;
        case R2: Serial.println(F("R2")); break;
        case L1: Serial.println(F("L1")); break;
        case R1: Serial.println(F("R1")); break;
        case TRIANGLE: Serial.println(F("TRIANGLE")); break;
        case CIRCLE: Serial.println(F("CIRCLE")); break;
        case CROSS: Serial.println(F("CROSS")); break;
        case SQUARE: Serial.println(F("SQUARE")); break;
        default: break;
    }
}

void GamepadController::dumpButtons(PsxButtons psxButtons) {
    static PsxButtons lastB = 0;
    if (psxButtons != lastB) {
        for (byte i = 0; i < PSX_BUTTONS_NO; ++i) {
            byte b = psxButtonToIndex(psxButtons);
            if (b < PSX_BUTTONS_NO) {
                controller(b);
            }
            psxButtons &= ~(1 << b);
            if (psxButtons == 0) break;
        }
        lastB = psxButtons;
    } else {
        controller(DEAF);
    }
}

void GamepadController::dumpAnalog(const char* str, byte x, byte y) {
    Serial.print(str);
    Serial.print(F(" analog: x = "));
    Serial.print(x);
    Serial.print(F(", y = "));
    Serial.println(y);
}

byte GamepadController::psxButtonToIndex(PsxButtons psxButtons) {
    for (byte i = 0; i < PSX_BUTTONS_NO; ++i) {
        if (psxButtons & 0x01) return i;
        psxButtons >>= 1U;
    }
    return DEAF;
}

byte GamepadController::getRightX() const {
    return srx;
}

