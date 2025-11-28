#include <Arduino.h>
#include "ultrasonic_sensor.h"

void ultrasonicInit() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    digitalWrite(TRIG_PIN, LOW);  // make sure trig is LOW
}

float readDistanceCM() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
    
    if (duration == 0) return -1; // no echo

    float distance = duration * 0.0343 / 2.0; // cm
    return distance;
}

float readWaterLevelCM() {
    float distance = readDistanceCM();
    if (distance < 0) return -1; // error

    float level = TANK_HEIGHT_CM - distance;
    if (level < 0) level = 0;
    if (level > TANK_HEIGHT_CM) level = TANK_HEIGHT_CM;

    return level;
}
