/*
  TODO
  - GPS
   - Theoretically implemented correctly
   - To be tested
  - Accelerometer
   - Everything
  What happens on connection lost

*/

#include <WiFi.h>
#include <FirebaseESP32.h>
#include <SoftwareSerial.h>
// #include <TinyGPS++.h>
#include "time.h"
#include "utilities.h"


// Pin declarations
const int radarPin = 13;


// Bird variables
// const int birdTimeLimit = 2000;         // Milliseconds of quiet required to log a bird event
bool bird = false;                      // Is a bird event ongoing
bool lastBirdState = false;             // For sending on falling edge
volatile unsigned long timeCounter = 0; // For checking time since last radar
volatile int radarTrigs = 0;            // Counter for how many times the radar has triggered


// NTP server timestamp variables
const char *ntpServer = "pool.ntp.org";

const bool useGPS = false;
const bool debug = false;


// ISR function
void radarEvent();

void setup() {

    // For debug purposes
    pinMode(12, OUTPUT);
    digitalWrite(12, HIGH);
    delay(1000);  

    
    // Establishing Serial communication
    Serial.begin(115200);

    // Setting up the interrupt
    attachInterrupt(digitalPinToInterrupt(radarPin), radarEvent, RISING);

    Serial.println(WIFI_SSID);
    Serial.println(WIFI_PASSWORD);

    // Connecting to WiFi
    connectWiFi();

    Serial.println("DEBUG: Wifi good");

    // Establishing connection with firebase
    connectFirebase();
    
    Serial.println("DEBUG: Firebase good");

    // Connecting to NTP server
    configTime(0, 0, ntpServer);
    delay (2000);

    // Setup GPS
    // gpsSerial.begin(9600);
    if (useGPS) updatePosition();
    // Sending the position to Firebase. Not implemented yet
    if (useGPS) Firebase.set(firebaseData, nodeName + "/position/center/lat", lat);
    if (useGPS) Firebase.set(firebaseData, nodeName + "/position/center/lng", lon);

    // Sends the new bootCount to Firebase, and increments by one if successful
    if (Firebase.set(firebaseData, nodeName + "/bootCount", bootCount + 1)) bootCount++;

    // Prints new bootCount to Serial monitor
    Serial.print("bootCount: ");
    Serial.println(bootCount);
}

void loop(){

    // Overflow safe comparison
    bird = millis() - timeCounter < birdTimeLimit;
    // On state change
    if (bird != lastBirdState) {
        // On rising edge of "bird"
        if (useGPS & bird) updatePosition();
        if (bird) UNIXtimestamp = getUnixTimestamp();  // Updates the timestamp
        if (bird) Serial.println(UNIXtimestamp);       // Prints timestamp for debug purposes
        // On falling edge of "bird"
        // if (radarTrigs > 2) Serial.println("BIRD!");
        if (radarTrigs > 2) sendToFirebase();
        if (!bird) radarTrigs = 0;
        lastBirdState = bird;
    }
}

// ISR function
void radarEvent() {
    timeCounter = millis();
    radarTrigs++;
}