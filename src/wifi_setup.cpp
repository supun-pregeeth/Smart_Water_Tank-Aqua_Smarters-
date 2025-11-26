#include <WiFi.h>
#include "wifi_setup.h"

#define WIFI_SSID "Pixel_9268"
#define WIFI_PASSWORD "057816222"

void connectToWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to WiFi");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 30) {
        Serial.print(".");
        delay(500);
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi Connected ✔");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\n❌ WiFi connection failed!");
    }
}
