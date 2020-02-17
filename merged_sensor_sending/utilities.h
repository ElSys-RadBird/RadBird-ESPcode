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

// NTP server timestamp variables
const char *ntpServer = "pool.ntp.org";


void connectWiFi();

String getTimestamp();

unsigned long getUnixTimestamp();