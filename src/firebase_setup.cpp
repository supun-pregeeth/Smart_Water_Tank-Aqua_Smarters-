#include "firebase_setup.h"

#define API_KEY "AIzaSyD9KlM0Gju77ZdvBEC4n8s0w75kwVLmABQ"
#define DATABASE_URL "https://test2-2c043-default-rtdb.firebaseio.com/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setupFirebase() {
    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    Serial.println("Firebase Initialized ✔");
}
