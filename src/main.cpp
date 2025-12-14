#include <WiFi.h>
#include "wifi_setup.h"
#include "firebase_setup.h"
#include "tds_sensor.h"
#include "turbidity_sensor.h"
#include "time.h" 
#include "flow_sensor.h"
#include "ultrasonic_sensor.h"
#include "solenoid_valve.h"
#include "ph_sensor.h"


// -------------------- NTP & Time --------------------
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800; // GMT+5:30
const int daylightOffset_sec = 0;

// -------------------- Sensor & Upload Timing --------------------
unsigned long prevSensorMillis = 0;
unsigned long prevUploadMillis = 0;

const unsigned long SENSOR_INTERVAL = 1000;   // 1 second
const unsigned long UPLOAD_INTERVAL = 2000;   // 2 seconds
 // 0.5 second poll for faster response


// -------------------- Sensor Values --------------------
float latestTDS = 0.0;
float latestTurbidity;
float latestFlowRate = 0.0;   
float totalVolume = 0.0;  
float latestWaterLevel = 0.0;
bool solenoidValue = false;
float phValue = 0.0;
String nowTime = "";


String getTimestamp() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "Time Error";
    }

    char buffer[10];
    // Format: YYYY-MM-DD HH:MM:SS
    sprintf(buffer,
        "%02d:%02d:%02d",
        timeinfo.tm_hour,
        timeinfo.tm_min,
        timeinfo.tm_sec
    );

    return String(buffer);
}


void setupTime() {
    
    /* Serial.println("Step 0: Synchronizing time with NTP..."); */
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    time_t now = time(nullptr); // get NTP time
    int attempts = 0;
    const int maxAttempts = 20;

    while (now < 1609459200 && attempts < maxAttempts) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
        attempts++;
    }

    if (now >= 1609459200) {
    Serial.println(" ✔ Time synchronized");

    struct tm *timeinfo = localtime(&now);  // convert to local time

    Serial.print("Current Date & Time: ");
    Serial.printf("%04d-%02d-%02d %02d:%02d:%02d\n",
              timeinfo->tm_year + 1900,  // year
              timeinfo->tm_mon + 1,      // month
              timeinfo->tm_mday,         // day
              timeinfo->tm_hour,         // hour
              timeinfo->tm_min,          // minute
              timeinfo->tm_sec);         // second

}
    else {
    Serial.println(" ❌ Time sync failed");
    }
}

void setup() {
    
    Serial.begin(115200);
    delay(500);

    nowTime = getTimestamp();

    Serial.println("------ SYSTEM START ------");

    Serial.println("Step 1: Connecting to WiFi...");
    connectToWiFi();

    Serial.println("Step 2: Synchronizing time with NTP...");
    setupTime();

    Serial.println("Step 3: Setting up Firebase...");
    setupFirebase();

    Serial.println("Step 4: Initializing flow sensor...");
    flowSensorInit( 23,390.0);

    Serial.println("Step 5: Initializing solenoid valve...");
    solenoidInit(13);

    Serial.println("Step 6: Initializing ultrasonic sensors...");
    ultrasonicInit();

    Serial.println("Step 7: Initializing pH sensor...");
    phInit();


}

void loop() {

    
    unsigned long currentMillis = millis();

    if (currentMillis - prevSensorMillis >= SENSOR_INTERVAL) {
        prevSensorMillis = currentMillis;

       /*  solenoidFirebaseControl();
        solenoidValue = getSolenoidState(); */

        float tdsReading = readTDS();
        if ( tdsReading  > 0 && tdsReading  < 2000) {          // valid TDS
        latestTDS = tdsReading;
        }

        /* float turbidityReading = readTurbidity();
        if ( turbidityReading  >= 0 && turbidityReading  < 1000) { // valid Turbidity
        latestTurbidity = turbidityReading;
        } */

        latestTurbidity = readTurbidity();
        latestWaterLevel = readWaterLevelCM();
        latestFlowRate = readFlow();
        phValue = readPH();
        uint32_t totalVolumeNow = getTotalVolume();
        uint32_t totalCount = getTotalPulses();
        
        
        /* solenoidFirebaseControl(); */   // <-- NEW: Firebase controls relay
        
        Serial.printf("------------------------------------------------------\n");
        Serial.printf("TDS: %.2f ppm\n", latestTDS);
        Serial.printf("Turbidity: %.2f NTU\n", latestTurbidity);
        Serial.printf("Water Level: %.2f cm\n", latestWaterLevel);
        Serial.printf("Flow Rate: %.2f L/min\n", latestFlowRate);
        Serial.printf("Total Volume: %.2f L\n", totalVolumeNow);
        Serial.printf("Timestamp: %s\n", nowTime.c_str());
        Serial.printf("pH Value: %.2f\n", phValue);
        Serial.printf("Solenoid State: %s\n", solenoidValue ? "ON" : "OFF");
        
        }

    // --- Upload to Firebase every 5 seconds ---
    if (currentMillis - prevUploadMillis >= UPLOAD_INTERVAL) {
        prevUploadMillis = currentMillis;

        Serial.print("Firebase signupOK: "); Serial.println(signupOK);
        Serial.print("Firebase ready: "); Serial.println(Firebase.ready());

        if (WiFi.status() == WL_CONNECTED && signupOK && Firebase.ready()) 
        {

            FirebaseJson dashboardJson;
            dashboardJson.add("flowRate", latestFlowRate);
            dashboardJson.add("valveStatus", solenoidValue);
            dashboardJson.add("waterLevel", latestWaterLevel);

            FirebaseJson qualityJson;
            qualityJson.add("ph", phValue); // replace with sensor reading if available
            qualityJson.add("tds", latestTDS);
            qualityJson.add("turbidity", latestTurbidity);

            FirebaseJson usageJson;
            usageJson.add("flowRate", latestFlowRate);
            usageJson.add("total_volume", totalVolume);
            usageJson.add("time", getTimestamp());
            

            String dashboardPath = "users/";
            dashboardPath += String(USER_UID);
            dashboardPath += "/dashboard";

            String qualityPath = "users/";
            qualityPath += String(USER_UID);
            qualityPath += "/quality";

            String usagePath = "users/";
            usagePath += String(USER_UID);
            usagePath += "/usage";

            Serial.print("Uploading data to Firebase path: ");
            Serial.println(dashboardPath);
            Serial.println(qualityPath);
            Serial.println(usagePath);


    if (Firebase.RTDB.setJSON(&fbdo, dashboardPath.c_str(), &dashboardJson)) {
        Serial.println("✅ Dashboard uploaded successfully!");
    } else {
        Serial.print("❌ Dashboard upload failed: ");
        Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.setJSON(&fbdo, qualityPath.c_str(), &qualityJson)) {
        Serial.println("✅ Quality uploaded successfully!");

    } else {
         Serial.print("❌ Quality upload failed: ");
        Serial.println(fbdo.errorReason());

    }

    if (Firebase.RTDB.setJSON(&fbdo, usagePath.c_str(), &usageJson)) {
        Serial.println("✅ TotleVolume uploaded successfully!");

    } else {
         Serial.print("❌ Quality upload failed: ");
        Serial.println(fbdo.errorReason());

    }

    }
}
}

// End of file: close loop() scope
