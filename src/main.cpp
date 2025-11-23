#include <WiFi.h>
#include "wifi_setup.h"
#include "firebase_setup.h"
#include "tds_sensor.h"
#include "turbidity_sensor.h"
#include "time.h" 

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800; // GMT+5:30
const int daylightOffset_sec = 0;

unsigned long prevSensorMillis = 0;
unsigned long prevUploadMillis = 0;

const unsigned long SENSOR_INTERVAL = 5000;   // 1 second
const unsigned long UPLOAD_INTERVAL = 10000;   // 5 seconds

float latestTDS = 0.0;
float latestTurbidity = 0.0;

void setupTime() {
    Serial.println("Step 0: Synchronizing time with NTP...");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    time_t now = time(nullptr);
    int attempts = 0;
    const int maxAttempts = 20;
    while (now < 1609459200 && attempts < maxAttempts) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
        attempts++;
    }

    if (now >= 1609459200) Serial.println(" ✔ Time synchronized");
    else Serial.println(" ❌ Time sync failed");
}

void setup() {
    Serial.begin(115200);
    Serial.println("------ SYSTEM START ------");

    Serial.println("Step 1: Connecting to WiFi...");
    connectToWiFi();

    Serial.println("Step 2: Synchronizing time with NTP...");
    setupTime();

    Serial.println("Step 3: Setting up Firebase...");
    setupFirebase();
}

void loop() {
    unsigned long currentMillis = millis();

    // --- Read sensors every 1 second ---
    if (currentMillis - prevSensorMillis >= SENSOR_INTERVAL) {
        prevSensorMillis = currentMillis;
        latestTDS = readTDS();
        latestTurbidity = readTurbidity();
        Serial.printf("Sensor Readings -> TDS: %.2f ppm, Turbidity: %.2f\n", latestTDS, latestTurbidity);
    }

    // --- Upload to Firebase every 5 seconds ---
    if (currentMillis - prevUploadMillis >= UPLOAD_INTERVAL) {
        prevUploadMillis = currentMillis;

        if (signupOK && Firebase.ready()) {
            FirebaseJson json;
            json.add("tds", latestTDS);
            json.add("turbidity", latestTurbidity);

            Serial.println("Uploading data to Firebase...");
            if (Firebase.RTDB.setJSON(&fbdo, "/water_quality", &json)) {
                Serial.println("✅ Data uploaded successfully!");
            } else {
                Serial.print("❌ Firebase upload failed: ");
                Serial.println(fbdo.errorReason());
            }
        } else {
            Serial.println("Firebase not ready or authentication failed. Skipping upload.");
        }
    }
}
