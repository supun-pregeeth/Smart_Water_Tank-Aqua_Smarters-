#ifndef FLOW_SENSOR_H
#define FLOW_SENSOR_H

#include <Arduino.h>

class FlowSensor {
public:
    FlowSensor(uint8_t pin = 12, float calFactor = 7.5); // Constructor initializes automatically

    float readFlow();
    float getTotalVolume();

private:
    static void IRAM_ATTR pulseCounterISR();
    static volatile uint32_t pulseCount;
    uint8_t sensorPin;
    float calibrationFactor;
    unsigned long lastTime;
    float totalLiters;
};

#endif
