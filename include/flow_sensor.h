#ifndef FLOW_SENSOR_H
#define FLOW_SENSOR_H

#include <Arduino.h>

// Pins & calibration
extern uint8_t flowPin;
extern float calibrationFactor;

// Flow sensor state
extern volatile uint32_t pulses;
/* volatile uint32_t totalPulses ; */
extern uint32_t totalPulses;
extern unsigned long lastTime;
extern float totalLiters;

// Functions
void flowSensorInit( uint8_t flowPin, float calibrationFactor); // Initialize flow sensor
float readFlow();          // Returns flow rate in L/min
float getTotalVolume();    // Returns total liters
float getTotalPulses();    // Returns total pulse count

#endif
