// Day 6 – ESP Web Rebooter Interface
// Access /reboot in browser to soft-restart the ESP

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

AsyncWebServer server(80);
const String auth_token = "1234";  // Change this for basic security

const char reboot_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP Rebooter</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { text-align: center; font-family: sans-serif; padding-top: 50px; background: #f5f5f5; }
    .btn { padding: 15px 30px; background: red; color: white; font-size: 20px; border: none; border-radius: 8px; cursor: pointer; }
    .box { background: white; padding: 30px; display: inline-block; box-shadow: 0px 0px 10px rgba(0,0,0,0.1); }
  </style>
</head>
<body>
  <div class="box">
    <h2>🔄 ESP Web Rebooter</h2>
    <form action="/confirm">
      <input type="hidden" name="auth" value="1234">
      <button class="btn" type="submit">Reboot Now</button>
    </form>
  </div>
</body>
</html>
)rawliteral";

const char rebooting_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head><meta http-equiv="refresh" content="5"><title>Rebooting...</title></head>
<body><h2 style='text-align:center;'>Rebooting ESP... Please wait</h2></body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP:");
  Serial.println(WiFi.localIP());

  server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", reboot_html);
  });

  server.on("/confirm", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("auth")) {
      String auth = request->getParam("auth")->value();
      if (auth == auth_token) {
        request->send_P(200, "text/html", rebooting_html);
        delay(1000);
        ESP.restart();
      } else {
        request->send(403, "text/plain", "Forbidden - Invalid Auth");
      }
    } else {
      request->send(403, "text/plain", "Forbidden - No Auth Param");
    }
  });

  server.begin();
}

void loop() {
  // Nothing to do here
}
