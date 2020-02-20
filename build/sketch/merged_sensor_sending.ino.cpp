#include <Arduino.h>
#line 1 "c:\\Users\\andgr\\Documents\\GitHub\\RadBird-ESPcode\\merged_sensor_sending\\merged_sensor_sending.ino"
#pragma once
#include <WiFi.h>
#include <FirebaseESP32.h>
#include "time.h"
#include "utilities.h"


// Pin declarations
const int radarPin = 13;
// int ledPin = pin number for debug LED
// const int gpsPin = ?
// const int accelerometerPin = ?

// Bird variables
const int birdTimeLimit = 2000;         // Milliseconds of quiet required to log a bird event
bool bird = false;
bool lastBirdState = false;
volatile unsigned long timeCounter = 0;


//Define Firebase Data object

// NTP server timestamp variables
const char *ntpServer = "pool.ntp.org";
int UNIXtimestamp;


void sendToFirebase();


// ISR function
void radarEvent();

#line 34 "c:\\Users\\andgr\\Documents\\GitHub\\RadBird-ESPcode\\merged_sensor_sending\\merged_sensor_sending.ino"
void setup();
#line 63 "c:\\Users\\andgr\\Documents\\GitHub\\RadBird-ESPcode\\merged_sensor_sending\\merged_sensor_sending.ino"
void loop();
#line 34 "c:\\Users\\andgr\\Documents\\GitHub\\RadBird-ESPcode\\merged_sensor_sending\\merged_sensor_sending.ino"
void setup() {
    // Setting up the interrupt
    attachInterrupt(digitalPinToInterrupt(radarPin), radarEvent, RISING);

    // Establishing Serial communication
    Serial.begin(115200);
    delay(1000);

    // Connecting to WiFi
    connectWiFi();

    // Establishing connection with firebase
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    // Connecting to NTP server
    configTime(0, 0, ntpServer);

    // Fetching the birdTimeLimit, bootCount and birdCount variables from Firebase
    fetchVariablesFromFirebase();

    // Sends the new bootCount to Firebase, and increments by one if successful
    if (Firebase.set(firebaseData, nodeName + "/bootCount", bootCount + 1)) bootCount++;

    // Prints new bootCount to Serial monitor
    Serial.print("bootCount: ");
    Serial.println(bootCount);
}

void loop(){

    bird = millis() - timeCounter < birdTimeLimit;
    // On falling edge of "bird"
    if (bird != lastBirdState) {
        if (bird) UNIXtimestamp = getUnixTimestamp(); // Updates the timestamp
        if (bird) Serial.println(UNIXtimestamp);
        if (!bird) sendToFirebase();
        lastBirdState = bird;
    }
}

// ISR function
void radarEvent() {
    timeCounter = millis();
}


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

    // Debug
    Serial.print("birdCount: ");
    Serial.println(birdCount);
}
