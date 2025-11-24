#include "flow_sensor.h"

volatile uint32_t FlowSensor::pulseCount = 0;

// ISR
void IRAM_ATTR FlowSensor::pulseCounterISR() {
    pulseCount++;
}

// Constructor — automatically sets up the sensor
FlowSensor::FlowSensor(uint8_t pin, float calFactor) {
    sensorPin = pin;
    calibrationFactor = calFactor;
    totalLiters = 0.0;
    lastTime = millis();
    pulseCount = 0;

    pinMode(sensorPin, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(sensorPin), pulseCounterISR, FALLING);

}

float FlowSensor::readFlow() {
    unsigned long now = millis();
    unsigned long dt = now - lastTime;
    if (dt == 0) return 0.0;

    float freq = pulseCount * (1000.0 / dt);
    float flowRate = freq / calibrationFactor;

    totalLiters += flowRate * (dt / 60000.0);

    pulseCount = 0;
    lastTime = now;

    return flowRate;
}

float FlowSensor::getTotalVolume() {
    return totalLiters;
}
