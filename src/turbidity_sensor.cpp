#include <Arduino.h>
#include "turbidity_sensor.h"

#define TURBIDITY_PIN 35

float readTurbidity() {

    int analogValue = analogRead(TURBIDITY_PIN);
    float voltage = analogValue * (3.3 / 4095.0);
    float turbidityNTU = -1120.4 * voltage * voltage + 5742.3 * voltage - 4352.9;

    return turbidityNTU;
}
