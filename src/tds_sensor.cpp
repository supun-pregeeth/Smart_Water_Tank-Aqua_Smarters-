#include <Arduino.h>

#define TDS_PIN 34
#define TDS_CALIBRATION 0.5   // Adjust based on calibration solution

float readTDS() {
    int analogValue = analogRead(TDS_PIN);           // 0-4095 for ESP32
    float voltage = analogValue * (3.3 / 4095.0);   // Convert ADC value to voltage

    // Voltage safety checks to prevent extreme values
    if (voltage < 0.1) voltage = 0.1;
    if (voltage > 3.0) voltage = 3.0;

    // Gravity/TDS sensor cubic formula
    float tdsValue = (133.42 * voltage * voltage * voltage
                     - 255.86 * voltage * voltage
                     + 857.39 * voltage) * TDS_CALIBRATION;

    // Clamp TDS to realistic range
    if (tdsValue < 0) tdsValue = 0;
    if (tdsValue > 2000) tdsValue = 2000;

    return tdsValue*3;
}
