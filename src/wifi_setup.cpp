#include <WiFi.h> // wifi library
#include "wifi_setup.h" //function declarations

#define WIFI_SSID "Pixel_9268"
#define WIFI_PASSWORD "057816222"

void connectToWiFi() {

    WiFi.mode(WIFI_STA); // Set WiFi to station mode
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Connect to WiFi network

    Serial.print("Connecting to WiFi");
    int attempts = 0; //WiFi.status() = WL_DISCONNECTED/WL_CONNECTED/WL_CONNECT_FAILED/WL_CONNECTION_LOST/WL_NO_SSID_AVAIL
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
