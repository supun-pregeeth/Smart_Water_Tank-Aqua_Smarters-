#include <Arduino.h>
#include "tds_sensor.h"

#define TDS_PIN 34

float readTDS() {
    int analogValue = analogRead(TDS_PIN);
    float voltage = analogValue * (3.3 / 4095.0);
    return (voltage * 1000) / 2.3;
}
