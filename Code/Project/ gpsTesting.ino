// NEO-6M data check - no library needed
// Wiring: GPS TX -> ESP32 GPIO16, GPS VCC -> 3V3, GND -> GND

#define GPS_RX_PIN 16
#define GPS_TX_PIN 17   // not needed for reading, safe to leave unconnected
#define GPS_BAUD   9600

HardwareSerial GPSSerial(2);

unsigned long totalBytes = 0;
unsigned long lastReport = 0;
String line = "";
String lastGGA = "";

void setup() {
  Serial.begin(115200);
  GPSSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  Serial.println("Listening to GPS (cold start can take 1-15 min)...");
}

void loop() {
  while (GPSSerial.available()) {
    char c = GPSSerial.read();
    totalBytes++;
    Serial.write(c);              // raw NMEA sentences
    if (c == '\n') {
      if (line.startsWith("$GPGGA") || line.startsWith("$GNGGA")) lastGGA = line;
      line = "";
    } else if (c != '\r') {
      line += c;
    }
  }

  if (millis() - lastReport >= 2000) {
    lastReport = millis();
    Serial.println();
    if (totalBytes == 0) {
      Serial.println(">> NO DATA: check GPS TX -> GPIO16, power, and GND.");
    } else {
      Serial.printf(">> DATA OK: %lu bytes received.\n", totalBytes);
      // GGA field 7 = fix quality (0 = no fix), field 8 = satellites used
      int f = 0, start = 0;
      String q = "?", sats = "?";
      for (int i = 0; i <= (int)lastGGA.length(); i++) {
        if (i == (int)lastGGA.length() || lastGGA[i] == ',') {
          String field = lastGGA.substring(start, i);
          f++;
          if (f == 7) q = field;
          if (f == 8) sats = field;
          start = i + 1;
        }
      }
      if (q == "0" || q == "?") Serial.printf(">> No fix yet (sats used: %s). Keep antenna outdoors.\n", sats.c_str());
      else Serial.printf(">> FIX ACQUIRED! Satellites: %s\n", sats.c_str());
    }
  }
}

