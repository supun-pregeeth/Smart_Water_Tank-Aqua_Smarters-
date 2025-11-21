#ifndef FIREBASE_SETUP_H
#define FIREBASE_SETUP_H

#include <Firebase_ESP_Client.h>

extern FirebaseData fbdo;
extern FirebaseAuth auth;
extern FirebaseConfig config;

void setupFirebase();

#endif
