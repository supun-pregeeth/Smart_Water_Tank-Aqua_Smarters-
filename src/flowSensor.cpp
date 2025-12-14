#include <Arduino.h>

uint8_t flowPin = 23;
float calibrationFactor = 390.0;

volatile uint32_t pulses = 0;
volatile uint32_t totalPulses = 0;
volatile uint32_t lastPulseMicros = 0;

unsigned long lastTime = 0;
float totalLiters = 0.0;
float lastFlowRate = 0.0;

void IRAM_ATTR flowPulseISR() {
    uint32_t now = micros();
    if (now - lastPulseMicros > 500) {
        pulses++;
        lastPulseMicros = now;
    }
}

void flowSensorInit(uint8_t pin, float calFactor) {
    flowPin = pin;
    calibrationFactor = calFactor;

    pulses = 0;
    totalPulses = 0;
    totalLiters = 0.0;
    lastTime = millis();

    pinMode(flowPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(flowPin), flowPulseISR, FALLING);
}

float readFlow() {
    unsigned long now = millis();
    unsigned long dt = now - lastTime;

    if (dt < 1000) return lastFlowRate;

    noInterrupts();
    uint32_t count = pulses;
    pulses = 0;
    interrupts();

    totalPulses += count;

    if (count == 0) {
        lastFlowRate = 0;
        lastTime = now;
        return 0;
    }

    float freq = count / (dt / 1000.0f);
    float rawFlow = (freq / calibrationFactor) * 60.0f;

    if (rawFlow < 0 || rawFlow > 50) rawFlow = 0;

    const float alpha = 0.25;
    float flowRate = alpha * rawFlow + (1 - alpha) * lastFlowRate;

    totalLiters += flowRate * (dt / 60000.0f);

    lastFlowRate = flowRate;
    lastTime = now;

    return flowRate;
}

float getTotalVolume() {
    return totalLiters;
}

uint32_t getTotalPulses() {
    noInterrupts();
    uint32_t t = totalPulses;
    interrupts();
    return t;
}
