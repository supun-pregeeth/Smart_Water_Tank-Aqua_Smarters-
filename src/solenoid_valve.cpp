#include "solenoid_valve.h"
#include <Firebase_ESP_Client.h>

// Extern values coming from main code or firebase_setup.cpp
extern FirebaseData fbdo;
extern bool signupOK;

// ---------------------------------------------------
// Variables
// ---------------------------------------------------
static int solenoidPin = -1;
static int solenoidState = 0;   // 0 = OFF, 1 = ON

// ---------------------------------------------------
// Initialization
// ---------------------------------------------------
void solenoidInit(int pin) {
    solenoidPin = pin;
    pinMode(solenoidPin, OUTPUT);
    digitalWrite(solenoidPin, LOW);
    solenoidState = 1;
    Serial.println("Solenoid valve initialized.");
}

// ---------------------------------------------------
// ON / OFF Functions
// ---------------------------------------------------
void solenoidOn() {
    digitalWrite(solenoidPin, HIGH);
    solenoidState = 1;
}

void solenoidOff() {
    digitalWrite(solenoidPin, LOW);
    solenoidState = 0;
}

// ---------------------------------------------------
// Return current state
// ---------------------------------------------------
int getSolenoidState() {
    return solenoidState;
}

// ---------------------------------------------------
// Firebase Remote Control
// Path: Water_quality/Aqua_Smatters/solenoid
// value: 1 = ON, 0 = OFF
// ---------------------------------------------------
void solenoidFirebaseControl() {
    if (!signupOK || !Firebase.ready()) return;

    int firebaseValue = 0;

    if (Firebase.RTDB.getInt(&fbdo, "Water_quality/Aqua_Smatters/solenoid")) {
        firebaseValue = fbdo.intData();

        if (firebaseValue == 1) {
            solenoidOn();
        } else {
            solenoidOff();
        }
    } else {
        Serial.print("Failed to read solenoid value: ");
        Serial.println(fbdo.errorReason());
    }
}
