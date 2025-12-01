#include <Arduino.h>

// --------- CONFIGURATION ---------
uint8_t flowPin = 23;             // ESP32 pin connected to YF-S201 yellow wire
float calibrationFactor =390; // Pulses per liter (replace with your measured value)


// --------- GLOBAL VARIABLES ---------
volatile uint32_t pulses = 0;     // Pulse counter (per interval)
volatile uint32_t totalPulses = 0;         // Total pulses counted
unsigned long lastTime = 0;       // Last time flow was calculated
float totalLiters = 0.0;          // Total accumulated water volume
float lastFlowRate = 0.0;         // Last calculated flow rate


// --------- ISR ---------
void IRAM_ATTR flowPulseISR() {
    pulses++;  // Increment pulse count on every pulse from sensor
}

// --------- Initialize flow sensor ---------
void flowSensorInit(uint8_t pin, float calFactor) {
    flowPin = pin;
    calibrationFactor = calFactor;
    totalLiters = 0.0;
    lastTime = millis();
    pulses = 0;
    totalPulses = 0;

    pinMode(flowPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(flowPin), flowPulseISR, FALLING);
}

// --------- Read flow rate (L/min) ---------
float readFlow() {
    unsigned long now = millis();
    unsigned long dt = now - lastTime; // Time difference in milliseconds
    if (dt == 0) return lastFlowRate;

    // Safely read and reset pulses
    noInterrupts();
    uint32_t count = pulses;
    pulses = 0;
    interrupts();

    totalPulses += count;  // accumulate total pulses

    // Pulses per second
    float freq = count * (1000.0f / dt);  

    // Flow rate in L/min
    float flowRate = (freq / calibrationFactor) * 60.0;

    // Accumulate total volume
    totalLiters += flowRate * (dt / 60000.0f);

    lastTime = now;
    lastFlowRate = flowRate;

    return flowRate;
}

// --------- Get total volume ---------
float getTotalVolume() {
    return totalLiters;
}

// --------- Get total pulses ---------
uint32_t getTotalPulses() {
    noInterrupts();
    uint32_t total = totalPulses;
    interrupts();
    return totalPulses;
}