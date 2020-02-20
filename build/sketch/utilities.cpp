#include "utilities.h"

// Globals
int bootCount;
int birdCount;
FirebaseData firebaseData;
const int nodeNumber = 1;   // Unique to each node deployed
const String nodeName = "node" + String(nodeNumber);


// WiFi connection setup
void connectWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) { // Sjekker om wifi er connected
        Serial.print(".");
        delay(300);
    }
    Serial.println("Hello");
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
}

// Returns the UNIX timestamp as an int. This works because int is 32 bit on ESP32
int getUnixTimestamp() {
    time_t now;
    time(&now);
    return int(now);
}

// Gets values from Firebase
void fetchVariablesFromFirebase() {

    // Get the bootCount from Firebase
    if (Firebase.getInt(firebaseData, nodeName + "/bootCount")) {
        if (firebaseData.dataType() == "int") bootCount = firebaseData.intData();
    }

    // Get the birdCount from Firebase
    if (Firebase.getInt(firebaseData, nodeName + "/birdCount")) {
        if (firebaseData.dataType() == "int") birdCount = firebaseData.intData();
    }
    
    // Get the birdTimeLimit from Firebase.
    // A test for storing variables for all nodes on Firebase, and for quicker editing of values
    // if (Firebase.getInt(firebaseData, "birdTimeLimit") & firebaseData.dataType() == int) const int birdTimeLimit = firebaseData.intData();
    // To be tested next time
}