#include <Arduino.h>
#include "ph_sensor.h"

#define PH_PIN 33   // Analog pin for pH sensor

void phInit() {
    pinMode(PH_PIN, INPUT);
}

float readPH() {
    int sensorValue = analogRead(PH_PIN);
    float voltage = sensorValue * (3.3 / 4095.0);   // Convert raw ADC to voltage

    // Basic pH formula (you can calibrate later)
    float ph = 7 + ((2.5 - voltage) * 3.5);

    return ph;
}