#include "utilities.h"

// This function returns the timestamp on hh:mm:ss format
String getTimestamp() {
    time_t rawtime;         // Test removing this later
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return "";
    }
    char timeStringBuff[64];
    strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M:%S", &timeinfo);
    String asString(timeStringBuff);
    return timeStringBuff;
}

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