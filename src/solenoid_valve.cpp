#include "solenoid_valve.h"
#include <Firebase_ESP_Client.h>
#include "firebase_setup.h"

// Firebase externs available via `firebase_setup.h`

// ---------------------------------------------------
// Variables
// ---------------------------------------------------
static int solenoidPin = -1;
static bool solenoidState = false;  // 0 = OFF, 1 = ON

// ---------------------------------------------------
// Initialization
// ---------------------------------------------------
void solenoidInit(int pin) {
    solenoidPin = pin;
    pinMode(solenoidPin, OUTPUT);
    digitalWrite(solenoidPin, LOW);
    solenoidState = false;
    Serial.println("Solenoid valve initialized.");
}

// ---------------------------------------------------
// ON / OFF Functions
// ---------------------------------------------------
void solenoidOn() {
    if (!solenoidState) {
        digitalWrite(solenoidPin, HIGH);
        solenoidState = true;
        Serial.println("Solenoid turned ON locally");
    }
}

void solenoidOff() {
    if (solenoidState) {
        digitalWrite(solenoidPin, LOW);
        solenoidState = false;
        Serial.println("Solenoid turned OFF locally");
    }
}

// ---------------------------------------------------
// Return current state
// ---------------------------------------------------
bool getSolenoidState(void) {
    return solenoidState;
}

// --------------------- ------------------------------
// Firebase Remote Control
// Reads valveStatus from dashboard every time this function is called
// ---------------------------------------------------
void solenoidFirebaseControl() {
    if (!signupOK || !Firebase.ready()) return;

    String solenoidPath = "users/";
    solenoidPath += String(USER_UID);
    solenoidPath += "/dashboard/valveStatus";
    Serial.print("Checking Firebase solenoid path: ");
    Serial.println(solenoidPath);

    bool updated = false;

    // Try to read as boolean
    if (Firebase.RTDB.getBool(&fbdo, solenoidPath.c_str())) {
        bool firebaseValue = fbdo.boolData();
        Serial.print("Firebase valveStatus (bool): ");
        Serial.println(firebaseValue ? "ON" : "OFF");

        if (firebaseValue) solenoidOn();
        else solenoidOff();
        updated = true;
    }

    // If not boolean, try to read as string (some apps send "true"/"false")
    if (!updated && Firebase.RTDB.getString(&fbdo, solenoidPath.c_str())) {
        String firebaseStr = fbdo.stringData();
        Serial.print("Firebase valveStatus (string): ");
        Serial.println(firebaseStr);

        if (firebaseStr.equalsIgnoreCase("true")) solenoidOn();
        else solenoidOff();
        updated = true;
    }

    if (!updated) {
        Serial.print("Failed to read solenoid value: ");
        Serial.println(fbdo.errorReason());
    }
}
