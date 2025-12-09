#include "solenoid_valve.h"
#include <Firebase_ESP_Client.h>
#include "firebase_setup.h"

// -------------------- Configuration --------------------
#define SOLENOID_ACTIVE_LOW true   // Set false if HIGH turns it ON

// -------------------- Globals --------------------
static int solenoidPin = -1;
static bool solenoidState = false;

// -------------------- Helper to set the GPIO --------------------
static void writeSolenoid(bool turnOn) {

    if (SOLENOID_ACTIVE_LOW) {
        digitalWrite(solenoidPin, turnOn ? LOW : HIGH);
    } else {
        digitalWrite(solenoidPin, turnOn ? HIGH : LOW);
    }

    solenoidState = turnOn;
}

// -------------------- Init --------------------
void solenoidInit(int pin) {
    solenoidPin = pin;
    pinMode(solenoidPin, OUTPUT);
    

    // Start OFF
    writeSolenoid(false);

    Serial.print("Solenoid initialized on pin ");
    Serial.println(solenoidPin);
}

// -------------------- Public API --------------------
void solenoidOn() {
    writeSolenoid(true);
    Serial.println("Solenoid ON");
}

void solenoidOff() {
    writeSolenoid(false);
    Serial.println("Solenoid OFF");
}

bool getSolenoidState() {
    return solenoidState;
}

// -------------------- Firebase Control --------------------
void solenoidFirebaseControl() {
    if (!signupOK || !Firebase.ready()) return;

    String path = "users/";
    path += String(USER_UID);
    path += "/dashboard/valveStatus";

    // Try boolean from Firebase
    if (Firebase.RTDB.getBool(&fbdo, path.c_str())) {
        bool value = fbdo.boolData();
        value ? solenoidOn() : solenoidOff();
        return;
    }

    // Try string (true/false, 1/0, on/off)
    if (Firebase.RTDB.getString(&fbdo, path.c_str())) {
        String s = fbdo.stringData();
        s.toLowerCase();

        if (s == "true" || s == "1" || s == "on")
            solenoidOn();
        else
            solenoidOff();

        return;
    }

    // Error reading
    Serial.print("Solenoid Firebase read error: ");
    Serial.println(fbdo.errorReason());
}
