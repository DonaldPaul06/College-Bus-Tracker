// ESP32 LED Blink

const int LED_PIN = 2;  // Built-in LED on many ESP32 boards

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH); // LED ON
  delay(1000);

  digitalWrite(LED_PIN, LOW);  // LED OFF
  delay(1000);
}
