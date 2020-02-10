#include <WiFi.h>
#include <FirebaseESP32.h>

#define WIFI_SSID "KatinkaErBest"
#define WIFI_PASSWORD "123456789"
#define FIREBASE_HOST "radbird-elsys.firebaseio.com" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "EWydgguCOq7ZUTcxpdHtaxLoA8NC5Z3PyLwlcs0q"


//Define Firebase Data object
FirebaseData firebaseData;


void setup()
{

    Serial.begin(115200);
    Serial.println();
    Serial.println();

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)//Sjekker om wifi er connected
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    //Setter opp en forbindelse med vår firebase
    Firebase.reconnectWiFi(true);
    //Kobler til automatisk til firebasen, usikker på når den gjør det 

    String path = "node1";

    String jsonStr = "";

    FirebaseJson json1; // Oppretter et json objekt 

    FirebaseJsonData jsonObj;

    json1.set("tid", 69);
    json1.set("aktivitet", true);
    json1.set("funker", false);
    
    

    Firebase.push(firebaseData, path, json1);


    json1.setJsonData(jsonStr);

    //Configure GPIO33 as ext0 wake up source for HIGH logic level
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,1);

    //Go to sleep now
    esp_deep_sleep_start();
}



void loop(){
  
  }
