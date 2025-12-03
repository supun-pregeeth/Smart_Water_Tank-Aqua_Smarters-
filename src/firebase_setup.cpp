#include "firebase_setup.h"

#define API_KEY "AIzaSyCKZolSWzhB-cDVnJX6G1gGAx40ltYbh74"
#define DATABASE_URL "https://test1-69fa1-default-rtdb.firebaseio.com"


FirebaseData fbdo; //Handles Firebase requests/responses
FirebaseAuth auth; //Handles authentication data
FirebaseConfig config; //Stores API key, DB URL, and other config for Firebase
bool signupOK = false;

void setupFirebase() {
    Serial.println("Initializing Firebase configuration...");

    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;

    Serial.println("Signing in to Firebase anonymously...");
    
    //Store token using auth


    //➡️ Sends signup request to Firebase
    //➡️ Firebase creates anonymous user
    //➡️ Firebase returns UID, ID token, refresh token
    //➡️ Tokens are stored inside auth
    //➡️ Function returns true if successful
    if (Firebase.signUp(&config, &auth, "", "")) { //Create token using signup method
        Serial.println("✅ Firebase authentication successful!");
        signupOK = true;
    } else {
        Serial.printf("❌ Firebase authentication failed: %s\n", config.signer.signupError.message.c_str());
        signupOK = false;
    }

    Firebase.begin(&config, &auth); //Use token from auth
    Firebase.reconnectWiFi(true); //Enable auto reconnect

    Serial.println("Firebase setup completed ✔");
}
