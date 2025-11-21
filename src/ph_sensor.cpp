#include <Arduino.h>
#include "ph_sensor.h"

#define PH_PIN 32   // adjust if using a different pin

float readPH() {
    int analogValue = analogRead(PH_PIN);
    float voltage = analogValue * (3.3 / 4095.0);  // ESP32 ADC to voltage

    // Basic formula (adjust with calibration later)
    float phValue = (voltage * 3.5);  // simple conversion for typical pH sensors

    return phValue;
}
