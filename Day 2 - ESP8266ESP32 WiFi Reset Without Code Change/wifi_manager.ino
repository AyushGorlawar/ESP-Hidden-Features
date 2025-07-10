// Day 2 – ESP WiFi Reset Without Code Change
// ESP8266 WiFi Manager using LittleFS or Embedded HTML

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
String ssid, password;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP WiFi Config</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial; text-align: center; padding: 30px; }
    input { padding: 8px; width: 80%%; margin: 10px 0; }
    button { padding: 10px 20px; font-size: 16px; }
  </style>
</head>
<body>
  <h2>Configure WiFi</h2>
  <form action="/save" method="POST">
    <input type="text" name="ssid" placeholder="Enter SSID" required><br>
    <input type="password" name="pass" placeholder="Enter Password" required><br>
    <button type="submit">Save & Reboot</button>
  </form>
</body>
</html>
)rawliteral";

void saveCredentials(String ssid, String password) {
  File file = SPIFFS.open("/wifi.txt", "w");
  if (file) {
    file.println(ssid);
    file.println(password);
    file.close();
  }
}

bool loadCredentials() {
  File file = SPIFFS.open("/wifi.txt", "r");
  if (!file) return false;
  ssid = file.readStringUntil('\n');
  password = file.readStringUntil('\n');
  ssid.trim();
  password.trim();
  return true;
}

void connectToWiFi() {
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.print("Connecting to WiFi");
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  if (loadCredentials()) {
    connectToWiFi();
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Switching to AP mode");
    WiFi.softAP("ESP_Config");
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/html", index_html);
    });

    server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request) {
      if (request->hasParam("ssid", true) && request->hasParam("pass", true)) {
        String new_ssid = request->getParam("ssid", true)->value();
        String new_pass = request->getParam("pass", true)->value();
        saveCredentials(new_ssid, new_pass);
        request->send(200, "text/plain", "Saved. Rebooting...");
        delay(1000);
        ESP.restart();
      } else {
        request->send(400, "text/plain", "Missing Params");
      }
    });

    server.begin();
  } else {
    Serial.println("WiFi connected! IP:");
    Serial.println(WiFi.localIP());
  }
}

void loop() {
  // nothing here
}
