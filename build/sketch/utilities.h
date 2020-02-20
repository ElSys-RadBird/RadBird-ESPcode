#pragma once
#include <WiFi.h>
#include <FirebaseESP32.h>
#include "time.h"

// Global defines
#define WIFI_SSID "networkTestAndreas"
#define WIFI_PASSWORD "123456789"
#define FIREBASE_HOST "radbird-elsys.firebaseio.com" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "EWydgguCOq7ZUTcxpdHtaxLoA8NC5Z3PyLwlcs0q"

// Global variables (?)
extern int bootCount;
extern int birdCount;
extern FirebaseData firebaseData;

extern const int nodeNumber;   // Unique to each node deployed
extern const String nodeName;


void connectWiFi();

int getUnixTimestamp();

void fetchVariablesFromFirebase();
