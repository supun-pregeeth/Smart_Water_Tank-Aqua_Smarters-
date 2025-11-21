#include <WiFi.h>
#include "wifi_setup.h"
#include "firebase_setup.h"
#include "tds_sensor.h"
#include "turbidity_sensor.h"

// ----------------- Setup -----------------
void setup() {
  Serial.begin(115200);

  connectToWiFi(); // First wifi
  setupFirebase(); // Second firebase
}

// ----------------- Loop -----------------
void loop() {
  
  static unsigned long prevMillis = 0;

  if (millis() - prevMillis >= 2000) {
    prevMillis = millis();

    float tds = readTDS();
    float turbidity = readTurbidity();

    Serial.printf("TDS: %.2f | Turbidity: %.2f\n", tds, turbidity);

    FirebaseJson json;
    json.add("tds", tds);
    json.add("turbidity", turbidity);

    Firebase.RTDB.setJSON(&fbdo, "/water_quality", &json);
  }
}
