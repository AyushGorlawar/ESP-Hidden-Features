// Day 4 – ESP Device Info Dashboard
// Displays system metrics via a web server UI

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
unsigned long startTime;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP Device Info</title>
  <meta http-equiv="refresh" content="5">
  <style>
    body { font-family: Arial; text-align: center; background: #f4f4f4; padding: 20px; }
    h2 { color: #333; }
    .box { background: white; padding: 20px; border-radius: 10px; display: inline-block; box-shadow: 0px 0px 10px rgba(0,0,0,0.1); }
    p { font-size: 18px; }
  </style>
</head>
<body>
  <div class="box">
    <h2>📊 ESP Device Info</h2>
    <p><strong>WiFi SSID:</strong> %SSID%</p>
    <p><strong>IP Address:</strong> %IP%</p>
    <p><strong>Signal Strength (RSSI):</strong> %RSSI% dBm</p>
    <p><strong>Uptime:</strong> %UPTIME%</p>
    <p><strong>Free Heap:</strong> %HEAP% bytes</p>
    <p><strong>Chip ID:</strong> %CHIPID%</p>
    <p><strong>Flash Size:</strong> %FLASH% bytes</p>
  </div>
</body>
</html>
)rawliteral";

String formatUptime(unsigned long ms) {
  unsigned long s = ms / 1000;
  unsigned int min = s / 60;
  unsigned int sec = s % 60;
  return String(min) + " mins " + String(sec) + " secs";
}

String processor(const String& var) {
  if (var == "SSID") return WiFi.SSID();
  if (var == "IP") return WiFi.localIP().toString();
  if (var == "RSSI") return String(WiFi.RSSI());
  if (var == "UPTIME") return formatUptime(millis() - startTime);
  if (var == "HEAP") return String(ESP.getFreeHeap());
  if (var == "CHIPID") return String(ESP.getChipId(), HEX);
  if (var == "FLASH") return String(ESP.getFlashChipSize());
  return String();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin("YOUR_SSID", "YOUR_PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.println(WiFi.localIP());

  startTime = millis();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  server.begin();
}

void loop() {
  // nothing to do here
}
