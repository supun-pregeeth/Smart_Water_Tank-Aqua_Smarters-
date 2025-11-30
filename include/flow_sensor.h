#ifndef FLOW_SENSOR_H
#define FLOW_SENSOR_H

#include <Arduino.h>

// Pins & calibration
extern uint8_t flowPin;
extern float calibrationFactor;

// Flow sensor state
extern volatile uint32_t pulses;
extern unsigned long lastTime;
extern float totalLiters;

// Functions
void flowSensorInit(uint8_t pin = 18, float calFactor = 7.5);
float readFlow();          // Returns flow rate in L/min
float getTotalVolume();    // Returns total liters

#endif
