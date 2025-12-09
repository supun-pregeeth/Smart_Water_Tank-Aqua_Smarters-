#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#define TRIG_PIN 26
#define ECHO_PIN 27
#define TANK_HEIGHT_CM 47 // change to your tank height

void ultrasonicInit();         // Initialize pins
float readDistanceCM();        // Read distance from sensor
float readWaterLevelCM();      // Calculate water level

#endif
