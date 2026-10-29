// Local bus tracker: ESP32 + NEO-6M GPS, shows live position on OpenStreetMap.
// No external server. Open  http://<ESP32-IP>/  from any phone/laptop on the same WiFi.
// Wiring: GPS TX -> GPIO16, GPS RX -> GPIO17, VCC -> 3.3V/5V, GND -> GND
// Library Manager: install "TinyGPSPlus" (Mikal Hart).
// Note: the phone/laptop viewing the page needs internet (for the map tiles), the ESP32 does not.

#include <WiFi.h>
#include <WebServer.h>
#include <TinyGPSPlus.h>

// ---------- EDIT THESE ----------
const char* WIFI_SSID = "wifi_name";
const char* WIFI_PASS = "wifi_pass";
const double HOME_LAT = 10.727779;   // college latitude
const double HOME_LNG = 76.290927;   // college longitude
const double RADIUS_M = 150;       // arrival radius in metres
// --------------------------------

TinyGPSPlus gps;
HardwareSerial gpsSerial(2);
WebServer server(80);

struct Ev { bool arrived; uint32_t t; };
Ev events[20];
int evCount = 0;
bool atCollege = false;

const char PAGE[] PROGMEM = R"HTML(
<!DOCTYPE html><html><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Bus Tracker</title>
<link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css">
<style>
body{margin:0;font-family:system-ui,sans-serif;display:flex;flex-direction:column;height:100vh}
#bar{padding:10px 14px;background:#222;color:#fff;display:flex;justify-content:space-between;align-items:center}
#st{padding:3px 10px;border-radius:99px;background:#888;font-size:13px}
#map{flex:1}#log{max-height:25vh;overflow:auto;padding:8px 14px;font-size:14px;background:#f5f5f5}
#ban{display:none;background:#16a34a;color:#fff;padding:10px;text-align:center;font-weight:600}
.bi{font-size:28px}
</style></head><body>
<div id="bar"><b>Bus Tracker</b><span id="st">Waiting for GPS...</span></div>
<div id="ban"></div><div id="map"></div><div id="log">No arrivals yet.</div>
<script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
<script>
var map=L.map('map').setView([10.0159,76.3419],15),bus=null,circ=null,first=true,seen=0;
L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png',{maxZoom:19,attribution:'© OpenStreetMap'}).addTo(map);
var icon=L.divIcon({html:'<div class="bi">🚌</div>',className:'',iconSize:[30,30],iconAnchor:[15,15]});
function tick(){fetch('/data').then(r=>r.json()).then(d=>{
 if(!circ){circ=L.circle(d.home,{radius:d.radius,color:'#16a34a',fillOpacity:.15}).addTo(map).bindTooltip('College');}
 var st=document.getElementById('st');
 if(d.fix){var p=[d.lat,d.lng];
  if(!bus)bus=L.marker(p,{icon:icon}).addTo(map);else bus.setLatLng(p);
  bus.bindTooltip(d.speed.toFixed(0)+' km/h');
  if(first){map.setView(p,16);first=false;}
  st.textContent=d.inside?'At college':'On the way ('+d.dist+' m away)';
  st.style.background=d.inside?'#16a34a':'#f59e0b';
 }else{st.textContent='Waiting for GPS ('+d.sats+' sats)';st.style.background='#888';
  if(first){map.setView(d.home,15);}}
 var l=document.getElementById('log');
 if(d.log.length){l.innerHTML=d.log.slice().reverse().map(e=>'<div>'+(e.a?'✅ Arrived':'➡️ Left')+' — '+new Date(e.t*1000).toLocaleString()+'</div>').join('');
  if(d.log.length>seen&&seen>0&&d.log[d.log.length-1].a){var b=document.getElementById('ban');b.textContent='🎉 Bus has arrived at college!';b.style.display='block';setTimeout(()=>b.style.display='none',8000);}
  seen=d.log.length;}
}).catch(()=>{});}
tick();setInterval(tick,2000);
</script></body></html>
)HTML";

long daysFromCivil(int y, int m, int d) {
  y -= m <= 2;
  long era = (y >= 0 ? y : y - 399) / 400;
  unsigned yoe = y - era * 400;
  unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
  unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
  return era * 146097L + (long)doe - 719468;
}

uint32_t gpsEpoch() {
  if (!gps.date.isValid() || !gps.time.isValid()) return 0;
  return daysFromCivil(gps.date.year(), gps.date.month(), gps.date.day()) * 86400UL +
         gps.time.hour() * 3600UL + gps.time.minute() * 60UL + gps.time.second();
}

void addEvent(bool arrived) {
  if (evCount == 20) { for (int i = 1; i < 20; i++) events[i - 1] = events[i]; evCount = 19; }
  events[evCount++] = { arrived, gpsEpoch() };
}

void handleData() {
  bool fix = gps.location.isValid();
  double dist = fix ? TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), HOME_LAT, HOME_LNG) : 0;
  String j = "{\"fix\":" + String(fix ? "true" : "false") +
             ",\"lat\":" + String(gps.location.lat(), 6) +
             ",\"lng\":" + String(gps.location.lng(), 6) +
             ",\"speed\":" + String(gps.speed.kmph(), 1) +
             ",\"sats\":" + String(gps.satellites.value()) +
             ",\"dist\":" + String((int)dist) +
             ",\"inside\":" + String(atCollege ? "true" : "false") +
             ",\"radius\":" + String(RADIUS_M, 0) +
             ",\"home\":[" + String(HOME_LAT, 6) + "," + String(HOME_LNG, 6) + "],\"log\":[";
  for (int i = 0; i < evCount; i++) {
    if (i) j += ",";
    j += "{\"a\":" + String(events[i].arrived ? "true" : "false") + ",\"t\":" + String(events[i].t) + "}";
  }
  j += "]}";
  server.send(200, "application/json", j);
}

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println();
  Serial.print("Open this on your phone/laptop: http://");
  Serial.println(WiFi.localIP());
  server.on("/", []() { server.send_P(200, "text/html", PAGE); });
  server.on("/data", handleData);
  server.begin();
}

void loop() {
  while (gpsSerial.available()) gps.encode(gpsSerial.read());
  server.handleClient();

  if (gps.location.isValid() && gps.location.isUpdated()) {
    double d = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), HOME_LAT, HOME_LNG);
    if (d <= RADIUS_M && !atCollege) { atCollege = true; addEvent(true); }
    else if (d > RADIUS_M + 20 && atCollege) { atCollege = false; addEvent(false); }  // +20 m margin avoids flicker
  }
}
