#ifndef SOLENOID_VALVE_H
#define SOLENOID_VALVE_H

#include <Arduino.h>

// Initializes the solenoid valve pin
void solenoidInit(int pin);

// Turn ON the solenoid valve
void solenoidOn();

// Turn OFF the solenoid valve
void solenoidOff();

// Get current solenoid status (1 = ON, 0 = OFF)
int getSolenoidState();

// Optional: Control solenoid using Firebase (if needed)
void solenoidFirebaseControl();

#endif
