#ifndef FIREBASE_SETUP_H
#define FIREBASE_SETUP_H

#include <Firebase_ESP_Client.h>

// Firebase objects
extern FirebaseData fbdo;
extern FirebaseAuth auth;
extern FirebaseConfig config;
extern bool signupOK;

// Initialize Firebase (anonymous signup)
void setupFirebase();

#endif
