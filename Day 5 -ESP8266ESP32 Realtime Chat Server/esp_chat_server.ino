// Day 5 – ESP8266/ESP32 Realtime Chat Server
// Lightweight LAN-based chatroom using AJAX

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

AsyncWebServer server(80);

String chatLog[10];
int messageCount = 0;

const char chat_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP Chat</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial; padding: 20px; background: #f2f2f2; }
    #chat { border: 1px solid #ccc; height: 300px; overflow-y: scroll; background: white; padding: 10px; }
    input { padding: 10px; width: 70%; }
    button { padding: 10px; }
  </style>
</head>
<body>
  <h2>💬 ESP Chat Room</h2>
  <div id="chat"></div>
  <br>
  <input type="text" id="msg" placeholder="Type a message">
  <button onclick="sendMsg()">Send</button>

  <script>
    function fetchChat() {
      fetch('/messages')
        .then(response => response.text())
        .then(data => {
          document.getElementById('chat').innerHTML = data;
        });
    }
    setInterval(fetchChat, 2000);
    function sendMsg() {
      let msg = document.getElementById('msg').value;
      fetch('/send?msg=' + encodeURIComponent(msg));
      document.getElementById('msg').value = '';
      setTimeout(fetchChat, 500);
    }
    fetchChat();
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", chat_html);
  });

  server.on("/messages", HTTP_GET, [](AsyncWebServerRequest *request){
    String content = "";
    for (int i = 0; i < messageCount; i++) {
      content += chatLog[i] + "<br>";
    }
    request->send(200, "text/html", content);
  });

  server.on("/send", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("msg")) {
      String msg = request->getParam("msg")->value();
      if (messageCount < 10) {
        chatLog[messageCount++] = msg;
      } else {
        for (int i = 1; i < 10; i++) {
          chatLog[i - 1] = chatLog[i];
        }
        chatLog[9] = msg;
      }
    }
    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
  // nothing here
}
