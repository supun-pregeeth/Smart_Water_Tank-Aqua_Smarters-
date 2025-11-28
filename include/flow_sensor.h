#ifndef FLOW_SENSOR_H
#define FLOW_SENSOR_H

#include <Arduino.h>

class FlowSensor {
public:
    FlowSensor(uint8_t pin = 12, float calFactor = 7.5);

    float readFlow();         // Returns flow rate (L/min)
    float getTotalVolume();   // Returns total volume (L)

private:
    static void IRAM_ATTR pulseISR();  // Interrupt function
    static volatile uint32_t pulses;   // Pulse counter (shared)

    uint8_t sensorPin;
    float calibrationFactor;
    unsigned long lastTime;
    float totalLiters;
};

#endif
