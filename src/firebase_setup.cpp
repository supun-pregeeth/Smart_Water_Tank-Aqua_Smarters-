#include "firebase_setup.h"

#define API_KEY "AIzaSyCKZolSWzhB-cDVnJX6G1gGAx40ltYbh74"
#define DATABASE_URL "https://test2-2c043-default-rtdb.firebaseio.com"


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;

void setupFirebase() {
    Serial.println("Initializing Firebase configuration...");

    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;

    Serial.println("Signing in to Firebase anonymously...");
    
    if (Firebase.signUp(&config, &auth, "", "")) {
        Serial.println("✅ Firebase authentication successful!");
        signupOK = true;
    } else {
        Serial.printf("❌ Firebase authentication failed: %s\n", config.signer.signupError.message.c_str());
        signupOK = false;
    }

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    Serial.println("Firebase setup completed ✔");
}
