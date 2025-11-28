#include <Arduino.h>
#include "tds_sensor.h"

#define TDS_PIN 34
#define TDS_CALIBRATION 0.5  // Adjust based on your calibration solution

float readTDS() {

    int analogValue = analogRead(TDS_PIN);              // 0-4095
    float voltage = analogValue * (3.3 / 4095.0);      // Convert to volts

    // Gravity/TDS sensor cubic formula (example)
    float tdsValue = (133.42 * voltage * voltage * voltage- 255.86 * voltage * voltage + 857.39 * voltage) * TDS_CALIBRATION;

    return tdsValue;
}
