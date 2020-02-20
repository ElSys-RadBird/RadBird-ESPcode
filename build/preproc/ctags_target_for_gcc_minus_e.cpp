# 1 "c:\\Users\\andgr\\Documents\\GitHub\\RadBird-ESPcode\\merged_sensor_sending\\merged_sensor_sending.ino"
/*

* TODO

* - GPS

*  - Everything

* - Get the birdTimeLimit from Firebase

* - Accelerometer

*  - Everything

* What happens on connection lost?

* 

*/
# 12 "c:\\Users\\andgr\\Documents\\GitHub\\RadBird-ESPcode\\merged_sensor_sending\\merged_sensor_sending.ino"
       
# 14 "c:\\Users\\andgr\\Documents\\GitHub\\RadBird-ESPcode\\merged_sensor_sending\\merged_sensor_sending.ino" 2
# 15 "c:\\Users\\andgr\\Documents\\GitHub\\RadBird-ESPcode\\merged_sensor_sending\\merged_sensor_sending.ino" 2
# 16 "c:\\Users\\andgr\\Documents\\GitHub\\RadBird-ESPcode\\merged_sensor_sending\\merged_sensor_sending.ino" 2
# 17 "c:\\Users\\andgr\\Documents\\GitHub\\RadBird-ESPcode\\merged_sensor_sending\\merged_sensor_sending.ino" 2


// Pin declarations
const int radarPin = 13;
// int ledPin = pin number for debug LED
// const int gpsPin = ?
// const int accelerometerPin = ?

// Bird variables
const int birdTimeLimit = 2000; // Milliseconds of quiet required to log a bird event
bool bird = false; // Is a bird event ongoing
bool lastBirdState = false; // For sending on falling edge
volatile unsigned long timeCounter = 0; // For checking time since last radar 


// NTP server timestamp variables
const char *ntpServer = "pool.ntp.org";



// ISR function
void radarEvent();

void setup() {

    // Establishing Serial communication
    Serial.begin(115200);

    // Setting up the interrupt
    attachInterrupt((((radarPin)<40)?(radarPin):-1), radarEvent, 0x01);

    // Connecting to WiFi
    connectWiFi();

    // Establishing connection with firebase
    connectFirebase();

    // Connecting to NTP server
    configTime(0, 0, ntpServer);

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
        if (bird) UNIXtimestamp = getUnixTimestamp(); // Updates the timestamp
        if (bird) Serial.println(UNIXtimestamp); // Prints timestamp for debug purposes
        // On falling edge of "bird"
        if (!bird) sendToFirebase();
        lastBirdState = bird;
    }
}

// ISR function
void radarEvent() {
    timeCounter = millis();
}
