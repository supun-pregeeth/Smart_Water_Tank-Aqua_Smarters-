#include "solenoid_valve.h"
#include <Firebase_ESP_Client.h>
#include "firebase_setup.h"
#include <Arduino.h>

// -------------------- Configuration --------------------
#define SOLENOID_ACTIVE_LOW true
static int solenoidPin = 13;
static bool solenoidState = false;

// Track last valve state to prevent flicker
static bool lastValveState = false;

// -------------------- GPIO Control --------------------
static void writeSolenoid(bool on) {
    digitalWrite(solenoidPin,
        SOLENOID_ACTIVE_LOW ? (on ? LOW : HIGH) : (on ? HIGH : LOW)
    );
    solenoidState = on;
}

// -------------------- Initialization --------------------
void solenoidInit(int pin) {
    solenoidPin = pin;
    pinMode(solenoidPin, OUTPUT);
    writeSolenoid(false); // OFF at start
    lastValveState = false;
    Serial.printf("Solenoid initialized on GPIO %d\n", solenoidPin);
}

// -------------------- API --------------------
void solenoidOn()  { writeSolenoid(true);  }
void solenoidOff() { writeSolenoid(false); }
bool getSolenoidState() { return solenoidState; }

// -------------------- Firebase Stream Callback --------------------
void solenoidStreamCallback(FirebaseStream data) {
    if (data.dataTypeEnum() == fb_esp_rtdb_data_type_boolean) {
        bool value = data.boolData();

        // Only act if value changed
        if (value != lastValveState) {
            writeSolenoid(value);
            lastValveState = value;

            Serial.printf("[solenoid STREAM] valveStatus = %s → %s\n",
                          value ? "true" : "false",
                          value ? "ON" : "OFF");
        }
    } else {
        Serial.println("[solenoid STREAM] Invalid data type received!");
    }
}

// -------------------- Stream Timeout Handler --------------------
void solenoidStreamTimeoutCallback(bool timeout) {
    if (timeout) Serial.println("[solenoid STREAM] Stream timeout!");
}

// -------------------- Start Firebase Streaming --------------------
void solenoidFirebaseStreamStart() {
    if (!signupOK || !Firebase.ready()) return;

    String path = String("users/");
    path += USER_UID;
    path += "/dashboard/valveStatus";

    if (!Firebase.RTDB.beginStream(&fbdo, path.c_str())) {
        Serial.printf("[solenoid STREAM] Stream begin failed: %s\n", fbdo.errorReason().c_str());
        return;
    }

    Firebase.RTDB.setStreamCallback(&fbdo, solenoidStreamCallback, solenoidStreamTimeoutCallback);
    Serial.println("[solenoid STREAM] Streaming started for valveStatus");
}

// -------------------- Manual Toggle --------------------
// Updates Firebase directly; stream will turn solenoid ON/OFF
void solenoidToggle() {
    if (!Firebase.ready()) return;

    bool newState = !lastValveState;

    String path = String("users/");
    path += USER_UID;
    path += "/dashboard/valveStatus";
    if (Firebase.RTDB.setBool(&fbdo, path.c_str(), newState)) {
        Serial.printf("[solenoid MANUAL] Toggled valve → %s\n", newState ? "ON" : "OFF");
    } else {
        Serial.printf("[solenoid MANUAL] Failed to toggle: %s\n", fbdo.errorReason().c_str());
    }
}
