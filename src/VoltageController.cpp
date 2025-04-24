#include "VoltageController.h"

VoltageController::VoltageController(uint8_t csPin, uint8_t loadPin)
    : _csPin(csPin), _loadPin(loadPin) {}

void VoltageController::begin() {
    pinMode(_csPin, OUTPUT);
    pinMode(_loadPin, OUTPUT);
    
    digitalWrite(_csPin, HIGH); 
    SPI.begin();  
}

void VoltageController::update(byte raw) {
    sendToMCP4922(computeBlue(raw));  
}

float VoltageController::computeBlue(byte raw) const {
   
    return map(raw, 0, 255, 180, 340) / 100.0;
}

void VoltageController::sendToMCP4922(uint8_t voltage) const {
 
}

