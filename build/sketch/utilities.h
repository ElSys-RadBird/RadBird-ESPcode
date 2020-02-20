#pragma once
#include <WiFi.h>
#include <FirebaseESP32.h>
#include "time.h"

// Global defines
#define WIFI_SSID "networkTestAndreas"
#define WIFI_PASSWORD "123456789"
#define FIREBASE_HOST "radbird-elsys.firebaseio.com" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "EWydgguCOq7ZUTcxpdHtaxLoA8NC5Z3PyLwlcs0q"

// Global variables. Make sure all of the externs are in the utilities.cpp as well!
extern int bootCount;               // Counter for how many times the system has booted
extern int birdCount;               // How many birds have been logged
extern FirebaseData firebaseData;   // Firebase Data object
extern const int nodeNumber;        // Which number the node has. Should perhaps be replaced with an ID
extern const String nodeName;       // Name of the node. Should perhaps be replaced with an ID
extern int UNIXtimestamp;           // The timestamp in UNIX time


void connectWiFi();

void connectFirebase();

int getUnixTimestamp();

int getIntFromFirebase(String variablePath);

void sendToFirebase();