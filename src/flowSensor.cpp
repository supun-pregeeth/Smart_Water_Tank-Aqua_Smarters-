#include "flow_sensor.h"

// Global variables
uint8_t flowPin = 23;
float calibrationFactor = 7.5;
volatile uint32_t pulses = 0;
unsigned long lastTime = 0;
float totalLiters = 0.0;

// ISR
void IRAM_ATTR flowPulseISR() {
    pulses++;
}

// Initialize sensor
void flowSensorInit(uint8_t pin, float calFactor) {
    flowPin = pin;
    calibrationFactor = calFactor;
    totalLiters = 0.0;
    lastTime = millis();
    pulses = 0;

    pinMode(flowPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(flowPin), flowPulseISR, FALLING);
}

// Read flow rate (L/min)
float readFlow() {
    unsigned long now = millis();
    unsigned long dt = now - lastTime;
    if (dt == 0) return 0.0;

    // safely read & reset pulses
    noInterrupts();
    uint32_t count = pulses;
    pulses = 0;
    interrupts();

    float freq = count * (1000.0f / dt);   // Hz
    float flowRate = freq / calibrationFactor;
    totalLiters += flowRate * (dt / 60000.0f); // accumulate total L

    lastTime = now;
    return flowRate;
}

// Return total volume
float getTotalVolume() {
    return totalLiters;
}
