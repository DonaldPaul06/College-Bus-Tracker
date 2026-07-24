#include <WiFi.h>

const char* ssid = "Your_WiFi_Name";
const char* password = "Your_WiFi_Password";

void setup() {
  Serial.begin(115200);

  Serial.print("Connecting to Wi-Fi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Your main code goes here
}
