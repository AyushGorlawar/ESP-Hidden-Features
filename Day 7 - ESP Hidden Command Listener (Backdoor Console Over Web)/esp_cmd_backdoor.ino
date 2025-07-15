// Day 7 – ESP Hidden Command Listener (Backdoor Console)
// Access: http://<IP>/cmd?token=ayush007&action=<command>

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const String auth_token = "ayush007";

AsyncWebServer server(80);
unsigned long startTime;

String formatUptime(unsigned long ms) {
  unsigned long s = ms / 1000;
  unsigned int min = s / 60;
  unsigned int sec = s % 60;
  return String(min) + "m " + String(sec) + "s";
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // LED off

  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected. IP:");
  Serial.println(WiFi.localIP());
  startTime = millis();

  server.on("/cmd", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!request->hasParam("token") || !request->hasParam("action")) {
      request->send(403, "text/plain", "Missing token or action");
      return;
    }

    String token = request->getParam("token")->value();
    String action = request->getParam("action")->value();

    if (token != auth_token) {
      request->send(403, "text/plain", "Invalid token");
      return;
    }

    if (action == "status") {
      String info = "Uptime: " + formatUptime(millis() - startTime);
      info += ", Heap: " + String(ESP.getFreeHeap());
      info += ", IP: " + WiFi.localIP().toString();
      request->send(200, "text/plain", info);

    } else if (action == "led_on") {
      digitalWrite(LED_BUILTIN, LOW);
      request->send(200, "text/plain", "OK: LED turned ON");

    } else if (action == "led_off") {
      digitalWrite(LED_BUILTIN, HIGH);
      request->send(200, "text/plain", "OK: LED turned OFF");

    } else if (action == "reboot") {
      request->send(200, "text/plain", "Rebooting now...");
      delay(1000);
      ESP.restart();

    } else if (action == "reset_wifi") {
      request->send(200, "text/plain", "WiFi credentials cleared. Rebooting...");
      delay(1000);
      ESP.eraseConfig();
      ESP.restart();

    } else if (action == "format_fs") {
      bool result = LittleFS.format();
      request->send(200, "text/plain", result ? "FS formatted." : "Format failed.");

    } else {
      request->send(400, "text/plain", "Unknown action: " + action);
    }
  });

  server.begin();
}

void loop() {
  // Hidden magic only
}
