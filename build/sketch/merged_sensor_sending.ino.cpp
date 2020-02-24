#include <Arduino.h>
#line 1 "c:\\Users\\andgr\\Documents\\GitHub\\RadBird-ESPcode\\merged_sensor_sending\\merged_sensor_sending.ino"
/*
* TODO
* - GPS
*  - Everything
*  - Replace tinyGPS library with tinyGPS++
* - Get the birdTimeLimit from Firebase
* - Accelerometer
*  - Everything
* What happens on connection lost?
* 
*/

#pragma once
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "time.h"
#include "utilities.h"


// Pin declarations
const int radarPin = 13;
// int ledPin = pin number for debug LED
// const int accelerometerPin = ?
const int gpsRXPin = 3;
const int gpsTXPin = 4;

// Bird variables
// const int birdTimeLimit = 2000;         // Milliseconds of quiet required to log a bird event
bool bird = false;                      // Is a bird event ongoing
bool lastBirdState = false;             // For sending on falling edge
volatile unsigned long timeCounter = 0; // For checking time since last radar 


// NTP server timestamp variables
const char *ntpServer = "pool.ntp.org";



// ISR function
void radarEvent();

#line 44 "c:\\Users\\andgr\\Documents\\GitHub\\RadBird-ESPcode\\merged_sensor_sending\\merged_sensor_sending.ino"
void setup();
#line 76 "c:\\Users\\andgr\\Documents\\GitHub\\RadBird-ESPcode\\merged_sensor_sending\\merged_sensor_sending.ino"
void loop();
#line 44 "c:\\Users\\andgr\\Documents\\GitHub\\RadBird-ESPcode\\merged_sensor_sending\\merged_sensor_sending.ino"
void setup() {
    
    // Establishing Serial communication
    Serial.begin(115200);

    // Setting up the interrupt
    attachInterrupt(digitalPinToInterrupt(radarPin), radarEvent, RISING);

    // Connecting to WiFi
    connectWiFi();

    // Establishing connection with firebase
    connectFirebase();

    // Connecting to NTP server
    configTime(0, 0, ntpServer);

    // Setup GPS
    gpsSerial.begin(9600);
    updatePosition();
    // Sending the position to Firebase. Not implemented yet
    Firebase.set(firebaseData, nodeName + "/position/lat", lat);
    Firebase.set(firebaseData, nodeName + "/position/lon", lon);

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
        if (bird) UNIXtimestamp = getUnixTimestamp();  // Updates the timestamp
        if (bird) Serial.println(UNIXtimestamp);       // Prints timestamp for debug purposes
        // On falling edge of "bird"
        if (!bird) sendToFirebase();
        lastBirdState = bird;
    }
}

// ISR function
void radarEvent() {
    timeCounter = millis();
}
