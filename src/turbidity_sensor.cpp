#include <Arduino.h>
#include "turbidity_sensor.h"

#define TURBIDITY_PIN 35

float readTurbidity() {

    int analogValue = analogRead(TURBIDITY_PIN);
    float voltage = analogValue * (3.3 / 4095.0);
    float turbidityNTU = -11.4 * voltage * voltage + 574.3 * voltage - 43.9 + 3.5;

    // Prevent negative turbidity

    return turbidityNTU*0.003;
}
