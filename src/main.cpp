#include <WiFi.h>
#include "wifi_setup.h"
#include "firebase_setup.h"
#include "tds_sensor.h"
#include "turbidity_sensor.h"
#include "time.h" 

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;  // your timezone offset in seconds (GMT+5:30)
const int daylightOffset_sec = 0;

void setupTime() {

    Serial.println("Step 0: Synchronizing time with NTP...");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    time_t now = time(nullptr);
    while (now < 1600000000) {  // wait until time is set
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
    Serial.println(" ✔ Time synchronized");
}

void setup() {
    Serial.begin(9600);
    Serial.println("------ SYSTEM START ------");

    Serial.println("Step 1: Connecting to WiFi...");
    connectToWiFi();

    Serial.println("Step 2: Setting up Firebase...");
    setupFirebase();
}

void loop() {
    static unsigned long prevMillis = 0;

    if (millis() - prevMillis >= 2000) {
        prevMillis = millis();

        Serial.println("Step 3: Reading sensors...");
        float tds = readTDS();
        float turbidity = readTurbidity();
        Serial.printf("Readings -> TDS: %.2f, Turbidity: %.2f\n", tds, turbidity);

        Serial.println("Step 4: Preparing JSON data...");
        FirebaseJson json;
        json.add("tds", tds);
        json.add("turbidity", turbidity);

        Serial.println("Step 5: Sending data to Firebase...");
        
        if (signupOK && Firebase.RTDB.setJSON(&fbdo, "/water_quality", &json)) {
            Serial.println("✅ Data uploaded successfully!");
        } else {
            Serial.print("❌ Firebase upload failed: ");
            Serial.println(fbdo.errorReason());
        }

        Serial.println("----------------------------");
    }
}
