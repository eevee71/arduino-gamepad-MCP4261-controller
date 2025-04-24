#include "VoltageController.h"

VoltageController::VoltageController(uint8_t csPin, uint8_t loadPin)
    : _csPin(csPin), _loadPin(loadPin) {}

void VoltageController::begin() {
    pinMode(_csPin, OUTPUT);
    pinMode(_loadPin, OUTPUT);
    
    digitalWrite(_csPin, HIGH);  // Zablokuj komunikację SPI (niskie aktywne)
    SPI.begin();  // Inicjalizacja SPI
}

void VoltageController::update(byte raw) {
    // Wyślij napięcie do MCP4922 na podstawie wartości odczytanej z gałki
    sendToMCP4922(computeBlue(raw));  // Tylko dla przykładu dla Blue
    // Możesz wysłać inne kolory lub zmienne, zależnie od Twoich potrzeb
}

float VoltageController::computeBlue(byte raw) const {
    // Mapa 0–255 → 1.8 V – 3.4 V
    return map(raw, 0, 255, 180, 340) / 100.0;
}

// Pozostałe metody takie jak computePurple, computeBrown, computeWhite pozostają bez zmian...

void VoltageController::sendToMCP4922(uint8_t voltage) const {
    // MCP4922 oczekuje 12-bitowych danych (2 bajty)
    // Należy przesłać odpowiedni kod SPI.
    
    // Formatuj dane dla MCP4922:
    // Bit 15 (MSB) - unipolar (0 dla 0-5V, 1 dla -5V do 5V)
    // Bit 14 - 3-bitowy kanał (0 dla kanal 1, 1 dla kanal 2)
    // Bit 13 - rejestr danych
    // Bit 12-1 - 10-bitowe dane
    uint16_t data = (0b0011 << 12) | (voltage << 2);  // Wyślij 10-bitowe dane na 12 bitach

    digitalWrite(_csPin, LOW);  // Aktywuj komunikację SPI (niskie aktywne)
    
    // Wyślij dane przez SPI (2 bajty)
    SPI.transfer(data >> 8);  // Wyślij wyższy bajt
    SPI.transfer(data & 0xFF);  // Wyślij niższy bajt
    
    digitalWrite(_csPin, HIGH);  // Dezaktywuj komunikację SPI
}

