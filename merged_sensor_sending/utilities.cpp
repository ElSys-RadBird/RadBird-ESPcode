#include "utilities.h"

// Global variables. Make sure all of the variables are declared as externs in the utilities.h as well!
int bootCount;
int birdCount;
FirebaseData firebaseData;
const int nodeNumber = 1;   // Unique to each node deployed
const String nodeName = "node" + String(nodeNumber);
int UNIXtimestamp;


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

// Firebase connection setup
void connectFirebase() {
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    // Fetching the birdTimeLimit, bootCount and birdCount variables from Firebase
    bootCount = getIntFromFirebase(nodeName + "/bootCount");
    birdCount = getIntFromFirebase(nodeName + "/birdCount");
    // birdTimeLimit = getIntFromFirebase("birdTimeLimit");    // This one is experimental and not yet working
    // A test for storing variables for all nodes on Firebase, and for quicker editing of values
    // To be tested next time

    Serial.print("Connected to Firebase on: ");
    Serial.println(FIREBASE_HOST);
}

// Returns the UNIX timestamp as an int. This works because int is 32 bit on ESP32
int getUnixTimestamp() {
    time_t now;
    time(&now);
    return int(now);
}

// Returns the value of an integer stored in variablePath on Firebase. Returns a 0 on error.
int getIntFromFirebase(String variablePath) {
    if (Firebase.getInt(firebaseData, variablePath) & firebaseData.dataType() == "int") return firebaseData.intData();
    else return 0;
}

// Sends a json object to Firebase
void sendToFirebase() { 

    // Increment the birdCount and send it to Firebase. Regardless of sending success
    Firebase.set(firebaseData, nodeName + "/birdCount", ++birdCount);

    // Generates a new node to write to, with a leading zero hard coded for all birds before the tenth
    String second_path;
    if (birdCount < 10) second_path = "bird0" + String(birdCount);
    else                second_path = "bird"  + String(birdCount);

    // Generating a json object 
    FirebaseJson json;

    // Appends the time, whether the unit is operational and eventually the position to the json object
    // Perhaps the position should only be updated at boot, and not at every bird event?
    // No, that would make the position a value sent too rarely to be a cool feature.
    json.set("tid", UNIXtimestamp);
    json.set("aktivitet", true);
    json.set("funker", false);
    
    // Sends the json object to Firebase
    Firebase.set(firebaseData, nodeName + "/" + second_path, json);

    // Prints how many birds have passed
    Serial.print("birdCount: ");
    Serial.println(birdCount);
}
