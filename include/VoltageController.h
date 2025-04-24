#ifndef VOLTAGECONTROLLER_H
#define VOLTAGECONTROLLER_H

#include <Arduino.h>
#include <SPI.h>

class VoltageController {
public:
    VoltageController(uint8_t csPin, uint8_t loadPin);

    void begin();
    void update(byte rawValue);  

private:
    uint8_t _csPin, _loadPin;

    float computeBlue(byte raw) const;
    float computePurple(byte raw) const;
    float computeBrown(byte raw) const;
    float computeWhite(byte raw) const;
    float computeControl() const;

    void sendToMCP4922(uint8_t data) const;

    static constexpr byte NEUTRAL_LOW  = 118;
    static constexpr byte NEUTRAL_HIGH = 138;
};

#endif // VOLTAGECONTROLLER_H
