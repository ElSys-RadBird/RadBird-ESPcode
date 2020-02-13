RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR int tall = 0;

#include <WiFi.h>
#include <FirebaseESP32.h>

#define WIFI_SSID "networkTestAndreas"
#define WIFI_PASSWORD "123456789"
#define FIREBASE_HOST "radbird-elsys.firebaseio.com" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "EWydgguCOq7ZUTcxpdHtaxLoA8NC5Z3PyLwlcs0q"

// Andreas shitcode goes here
int radarPin = 13;
// int ledPin = LED_BUILTIN;
volatile bool activity;
bool bird = false;
const int birdTimeLimit = 2000;
unsigned long timeCounter = 0;

bool lastBirdState = false;

void radarEvent();
void functionTest();

// Andreas shitcode ends here


//Define Firebase Data object
FirebaseData firebaseData;


void setup(){
  // More Andreas shitcode goes here
  // pinMode(ledPin, OUTPUT);
  // Can I swap this with the deep sleep interrupt mode?
  // attachInterrupt(digitalPinToInterrupt(radarPin), radarEvent, RISING);

  // More Andreas shitcode ends

  Serial.begin(115200);
  delay(1000); 

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)//Sjekker om wifi er connected
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println("Hello");
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    //Setter opp en forbindelse med vår firebase
    Firebase.reconnectWiFi(true);
    //Kobler til automatisk til firebase, usikker på når den gjør det 

  // Why is this here? This will only run at startup, and unless we can store the bootCount offline, it will not help
  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  /*
  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  //Configure GPIO33 as ext0 wake up source for HIGH logic level
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,1);
  
  //Go to sleep now
  esp_deep_sleep_start();
  */

  // Attempt from Andreas at sleep_mode
  print_wakeup_reason();

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 1);

  esp_deep_sleep_start();
}
 
void loop(){

  /*
  if (activity) {
    timeCounter = millis();
    activity = false;
  }
  */
}
// Attempted sleep wakeup function from Andreas
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason) {
    case 2:
      Serial.println("Wakeup caused by external signal using RTC_CNTL"); 
      activity = true;
      timeCounter = millis();
      onWakeupFunction();
      break;
    default:
      Serial.println("Wakeup was not caused by deep sleep");
      break;
  }
}

/*
//Function that prints the reason by which ESP32 has been awaken from sleep
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason){
    case 2: Serial.println("Wakeup caused by external signal using RTC_CNTL"); 
      break;
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }
  function_test();
}
  */

void onWakeupFunction() {
  while (millis() - timeCounter < birdTimeLimit) {
    bird = millis() - timeCounter < birdTimeLimit;
    
    if (bird != lastBirdState) {
      lastBirdState = bird;
      if (!bird) {
        function_test();
        esp_deep_sleep_start();
      }
    }
  }
}



void radarEvent() {
  activity = true;
}

void function_test() { 
  ++tall;
  String path = "node1";
  String second_path = "timestamp" + String(tall);
  String jsonStr = "";

  FirebaseJson json1; // Oppretter et json objekt 
  FirebaseJson json2; // Hvorfor 2 objekter?

  json1.set("tid", tall);
  json1.set("aktivitet", true);
  json1.set("funker", false);
  
  FirebaseJsonData jsonObj;

  Firebase.set(firebaseData, path + "/" + second_path, json1); 
  Serial.print("tall: ");
  Serial.println(tall);
}
