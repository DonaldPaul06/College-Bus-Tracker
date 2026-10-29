# Local Bus Tracker (ESP32 + GPS + OpenStreetMap)

A single-file Arduino sketch that turns an ESP32 and a NEO-6M GPS module into a self-hosted bus tracker. The ESP32 serves its own web page, so there is no external server, database or Node.js. Open the page from any phone or laptop on the same WiFi to see the bus live on OpenStreetMap, get notified when it reaches the college, and view a log of arrival times.

## Features

- Live bus position on an OpenStreetMap map (updates every 2 seconds)
- Shows speed, satellite count and distance to the college
- Green circle on the map marking the college arrival radius
- Automatic arrival detection: logs **Arrived** when the bus enters the radius and **Left** when it exits
- Arrival and departure times taken from the GPS clock (no internet clock needed)
- On-screen banner when the bus arrives while the page is open
- Event log of the last 20 arrivals/departures
- Runs entirely on your local network

## Hardware

| Part | Notes |
|------|-------|
| ESP32 dev board | Any common ESP32 DevKit |
| NEO-6M GPS module | With antenna |
| Jumper wires | |
| 5V USB power | Power bank or vehicle USB adapter |

### Wiring

| GPS module | ESP32 |
|------------|-------|
| VCC | 3.3V (or 5V, depending on your module) |
| GND | GND |
| TX | GPIO16 |
| RX | GPIO17 |

## Software setup

1. Install the **Arduino IDE** and add ESP32 board support (Boards Manager, search "esp32").
2. Install the **TinyGPSPlus** library by Mikal Hart (Library Manager).
3. Open `bus_tracker_local.ino`.
4. Edit the settings at the top of the file:

```cpp
const char* WIFI_SSID = "YOUR_WIFI";
const char* WIFI_PASS = "YOUR_PASSWORD";
const double HOME_LAT = 10.0159;   // college latitude
const double HOME_LNG = 76.3419;   // college longitude
const double RADIUS_M = 150;       // arrival radius in metres
```

5. Select your ESP32 board and port, then upload.
6. Open the Serial Monitor at **115200 baud**. It prints the address, for example `http://192.168.1.45`.

## Using it

1. Place the GPS antenna with a clear view of the sky (near a window or outdoors). The first fix can take a minute or two.
2. On a phone or laptop connected to the **same WiFi**, open the address shown in the Serial Monitor.
3. The page shows:
   - The bus marker, moving live
   - The college circle
   - A status badge: grey (waiting for GPS), orange (on the way, with distance), green (at college)
   - The arrival log, newest first

To find your college coordinates, right-click the spot in Google Maps or OpenStreetMap and copy the latitude and longitude.

## How it works

- The ESP32 reads NMEA data from the GPS on a hardware serial port and decodes it with TinyGPSPlus.
- A small web server on port 80 serves two routes:
  - `/` returns the map page (Leaflet and OpenStreetMap tiles load from the internet in the viewer's browser)
  - `/data` returns live JSON: position, speed, satellites, distance, status, home location and the event log
- On each new GPS position the sketch measures the distance to `HOME_LAT`/`HOME_LNG`:
  - Inside `RADIUS_M` and previously outside: logs **Arrived**
  - More than 20 m outside the radius and previously inside: logs **Left** (the 20 m margin prevents flicker from GPS drift)
- Event times are stored as Unix timestamps from the GPS date/time and shown in the viewer's local time zone.

## Limitations

- **One bus per ESP32.** For several buses, run one unit per bus (each has its own IP), or build a central server version.
- **Local WiFi only.** The ESP32 must be on a network the viewer can reach. For a moving bus, use a phone hotspot or 4G router on the bus, with the viewing device on the same network.
- **The viewing device needs internet** to load the map tiles and the Leaflet library. The ESP32 itself does not.
- **The log is kept in memory** and is lost when the ESP32 restarts.
- **No password or admin panel.** Change the college location and radius in the code and re-upload.
- **No sound or push notifications**, only the on-screen banner.
- The first-arrival banner may not appear if the page was opened while the log was empty (the log still updates). Events logged before the GPS has a valid date may show a 1970 timestamp.

## Troubleshooting

| Problem | What to check |
|---------|---------------|
| Serial Monitor shows only dots | Wrong WiFi name/password, or the network is 5 GHz only (ESP32 needs 2.4 GHz) |
| Page says "Waiting for GPS" | Take the antenna outdoors or near a window; wait a few minutes; check TX/RX wiring (GPS TX goes to GPIO16) |
| Map is grey or blank | The viewing device has no internet access |
| Page won't open | Phone/laptop is on a different network, or the IP changed after a reboot |
| Arrival not logging | Increase `RADIUS_M` (use at least 100 m), and double-check the college coordinates |

## Credits

- [TinyGPSPlus](https://github.com/mikalhart/TinyGPSPlus) by Mikal Hart
- [Leaflet](https://leafletjs.com/) for the map interface
- Map data © [OpenStreetMap](https://www.openstreetmap.org/copyright) contributors
