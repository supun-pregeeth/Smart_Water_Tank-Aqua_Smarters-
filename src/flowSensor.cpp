#include "flow_sensor.h"

volatile uint32_t FlowSensor::pulses = 0;

// ISR - runs automatically when pulse detected
void IRAM_ATTR FlowSensor::pulseISR() {
    pulses++;
}

// Constructor
FlowSensor::FlowSensor(uint8_t pin, float calFactor) {
    sensorPin = pin;
    calibrationFactor = calFactor;
    totalLiters = 0.0;
    lastTime = millis();
    pulses = 0;

    pinMode(sensorPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(sensorPin), pulseISR, FALLING);
}

// Read flow rate (L/min)
float FlowSensor::readFlow() {
    unsigned long now = millis();
    unsigned long dt = now - lastTime;

    if (dt == 0) return 0.0;

    // safely copy & reset pulse count
    noInterrupts();
    uint32_t count = pulses;
    pulses = 0;
    interrupts();

    // frequency (Hz)
    float freq = count * (1000.0f / dt);

    // convert to L/min
    float flowRate = freq / calibrationFactor;

    // accumulate total liters
    totalLiters += flowRate * (dt / 60000.0f);

    lastTime = now;
    return flowRate;
}

// Return total volume
float FlowSensor::getTotalVolume() {
    return totalLiters;
}
