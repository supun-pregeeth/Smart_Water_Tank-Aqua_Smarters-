#include <Arduino.h>
#include "turbidity_sensor.h"

#define TURBIDITY_PIN 35

float readTurbidity() {
    int analogValue = analogRead(TURBIDITY_PIN);
    float voltage = analogValue * (3.3 / 4095.0);
    return 3000 - (voltage * 1000);
}
