#include "solenoid_valve.h"
#include <Arduino.h>

SolenoidValve::SolenoidValve(int pin) {
    relayPin = pin;
    state = false;
}

void SolenoidValve::begin() {
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW); // Default OFF
}

void SolenoidValve::open() {
    digitalWrite(relayPin, HIGH);
    state = true;
}

void SolenoidValve::close() {
    digitalWrite(relayPin, LOW);
    state = false;
}

bool SolenoidValve::isOpen() {
    return state;
}

// --- New function to control valve based on water level ---
void SolenoidValve::update(float waterLevel) {
    if (waterLevel < 50.0 && !state) {
        open();
        Serial.println("Solenoid Valve OPENED");
    } 
    else if (waterLevel >= 50.0 && state) {
        close();
        Serial.println("Solenoid Valve CLOSED");
    }

    Serial.printf("Current Water Level: %.2f cm\n", waterLevel);
}
