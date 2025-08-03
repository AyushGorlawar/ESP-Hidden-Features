
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>

const char* ssid = "YourSSID";
const char* password = "YourPassword";

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

int sensorPin = A0;
int sensorValue = 0;

String htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Real-Time Dashboard</title>
  <script>
    var connection = new WebSocket('ws://' + location.hostname + ':81/');
    connection.onmessage = function (event) {
      document.getElementById('sensor').innerText = event.data;
    };
  </script>
</head>
<body>
  <h1>Real-Time Sensor Dashboard</h1>
  <p>Sensor Value: <span id="sensor">--</span></p>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/html", htmlPage);
  });

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  server.handleClient();
  webSocket.loop();

  sensorValue = analogRead(sensorPin);
  webSocket.broadcastTXT(String(sensorValue));
  delay(1000);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  // No specific action for client events in this project
}
