RTC_DATA_ATTR int bootCount = 0;
extern int tall = 0;

#include <WiFi.h>
#include <FirebaseESP32.h>
#include "time.h"
#include "utilities.h"

// #define WIFI_SSID "networkTestAndreas"
// #define WIFI_PASSWORD "123456789"
// #define FIREBASE_HOST "radbird-elsys.firebaseio.com" //Do not include https:// in FIREBASE_HOST
// #define FIREBASE_AUTH "EWydgguCOq7ZUTcxpdHtaxLoA8NC5Z3PyLwlcs0q"

const int radarPin = 13;
// int ledPin = pin number for debug LED
const int birdTimeLimit = 2000;
unsigned long timestamp;

bool bird = false;
bool lastBirdState = false;
volatile unsigned long timeCounter = 0;

void radarEvent();
void sendToFirebase();



//Define Firebase Data object
FirebaseData firebaseData;


void setup() {
    // Setting up the interrupt
    attachInterrupt(digitalPinToInterrupt(radarPin), radarEvent, RISING);

    Serial.begin(115200);
    delay(1000);

    connectWiFi();

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    //Setter opp en forbindelse med vår firebase
    Firebase.reconnectWiFi(true);
    //Kobler til automatisk til firebase, usikker på når den gjør det

    // Connecting to NTP server
    configTime(0, 0, ntpServer);

    // Why is this here? This only works if using deep sleep. I am against using that
    //Increment boot number and print it every reboot
    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));
}
 
void loop(){

    bird = millis() - timeCounter < birdTimeLimit;
    // On falling edge of "bird"
    if (bird != lastBirdState) {
        timestamp = getUnixTimestamp(); // Updates the timestamp
        if (!bird) sendToFirebase();
        lastBirdState = bird;
    }
}

// ISR function
void radarEvent() {
    timeCounter = millis();
}

void sendToFirebase() { 
  ++tall;
  String path = "node1";
  String second_path = "timestamp" + String(tall);
  String jsonStr = "";

  FirebaseJson json1; // Oppretter et json objekt 
  FirebaseJson json2; // Hvorfor 2 objekter?

  json1.set("tid", timestamp);
  json1.set("aktivitet", true);
  json1.set("funker", false);
  
  FirebaseJsonData jsonObj;

  Firebase.set(firebaseData, path + "/" + second_path, json1);

  // Debug
  Serial.print("tall: ");
  Serial.println(tall);
}

