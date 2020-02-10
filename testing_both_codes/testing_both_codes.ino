RTC_DATA_ATTR int bootCount = 0;


#include <WiFi.h>
#include <FirebaseESP32.h>

#define WIFI_SSID "KatinkaErBest"
#define WIFI_PASSWORD "123456789"
#define FIREBASE_HOST "radbird-elsys.firebaseio.com" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "EWydgguCOq7ZUTcxpdHtaxLoA8NC5Z3PyLwlcs0q"


//Define Firebase Data object
FirebaseData firebaseData;


void setup(){
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
    //Kobler til automatisk til firebasen, usikker på når den gjør det 
    
    

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  Serial.println("Ferdig med setup");

  //Print the wakeup reason for ESP32
  print_wakeup_reason();
Serial.println("Ferdig med setup2");
  //Configure GPIO33 as ext0 wake up source for HIGH logic level
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,1);
Serial.println("Ferdig med setup3");
  //Go to sleep now
  esp_deep_sleep_start();
  Serial.println("Ferdig med setup4");
}



void function_test() { 
    String path = "node1";

    String jsonStr = "";

    FirebaseJson json1; // Oppretter et json objekt 

    FirebaseJsonData jsonObj;

    json1.set("tid", 69);
    json1.set("aktivitet", true);
    json1.set("funker", false);
  Firebase.set(firebaseData, path, json1); 
  };

  

void loop(){
  Serial.println("Jeg er inni loopen");
  }

//Function that prints the reason by which ESP32 has been awaken from sleep
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason){
    //case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case 2: Serial.println("Wakeup caused by external signal using RTC_CNTL"); 
    break;
    //case 3  : Serial.println("Wakeup caused by timer"); break;
    //case 4  : Serial.println("Wakeup caused by touchpad"); break;
    //case 5  : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}
