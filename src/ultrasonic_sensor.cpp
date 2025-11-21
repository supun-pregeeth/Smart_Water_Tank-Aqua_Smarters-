#include <Arduino.h>
#include "ultrasonic_sensor.h"

#define TRIG_PIN 26
#define ECHO_PIN 27
#define TANK_HEIGHT_CM 100   // change to your tank height

float readDistanceCM() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    float distance = duration * 0.034 / 2; // distance in cm

    return distance;
}

float readWaterLevelCM() {
    float distance = readDistanceCM();
    float level = TANK_HEIGHT_CM - distance;
    
    if (level < 0) level = 0;
    if (level > TANK_HEIGHT_CM) level = TANK_HEIGHT_CM;

    return level;
}
