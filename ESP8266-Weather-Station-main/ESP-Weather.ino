#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>  

// OpenWeather API Configuration
const char* openWeatherApiKey = "b30c6e539e0c9a84e90ba91b0524756"; // Your OpenWeather API key
const char* defaultCity = "Pune";              // Default city
const char* defaultCountryCode = "IN";         // Default country code

String currentCity = defaultCity;              // Current city (can be changed by user)
String currentCountryCode = defaultCountryCode; // Current country code (can be changed by user)

// Server setup
ESP8266WebServer server(80);
unsigned long lastWeatherUpdate = 0;
const unsigned long updateInterval = 600000;   // Update every 10 minutes (600,000ms)

// Weather data storage
struct WeatherData {
  float temperature;
  float feelsLike;
  int humidity;
  float windSpeed;
  String weatherDescription;
  String weatherIcon;
  long lastUpdate;
  bool dataValid;
} currentWeather;

void setup() {
  Serial.begin(115200);
  
  // WiFiManager - handles connecting to WiFi
  WiFiManager wifiManager;
  
  // Uncomment to reset saved settings
  //wifiManager.resetSettings();
  
  // Creates an access point named "ESP-Weather-Setup" if can't connect to saved WiFi
  // You'll connect to this AP to enter your WiFi credentials
  if(!wifiManager.autoConnect("ESP-Weather-Setup")) {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    // Reset and try again
    ESP.reset();
    delay(5000);
  }
  
  // If you reach here, you're connected to WiFi with internet
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Server routes
  server.on("/", handleRoot);
  server.on("/refresh", handleRefresh);
  server.on("/style.css", handleCSS);
  server.on("/weather", handleWeatherData);
  server.on("/changeLocation", HTTP_POST, handleLocationChange);
  server.on("/resetLocation", handleResetLocation);
  server.on("/resetWiFi", handleWiFiReset);  // New route for WiFi reset
  
  // Start the server
  server.begin();
  Serial.println("HTTP server started");
  
  // Initialize weather data
  currentWeather.dataValid = false;
  
  // Initial weather fetch
  fetchWeatherData();
}

void loop() {
  server.handleClient();
  
  // Update weather data every 10 minutes
  if (millis() - lastWeatherUpdate > updateInterval) {
    fetchWeatherData();
  }
}

void fetchWeatherData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;
    
    String url = "http://api.openweathermap.org/data/2.5/weather?q=" + 
                 currentCity + "," + currentCountryCode + 
                 "&units=metric&appid=" + String(openWeatherApiKey);
    
    http.begin(client, url);
    int httpCode = http.GET();
    
    if (httpCode == 200) {
      String payload = http.getString();
      parseWeatherData(payload);
      currentWeather.dataValid = true;
      Serial.println("Weather data fetched successfully for " + currentCity);
    } else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpCode);
      currentWeather.dataValid = false;
    }
    
    http.end();
  } else {
    Serial.println("WiFi disconnected, attempting to reconnect...");
    WiFi.begin(); // Try to reconnect using saved credentials
    
    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 20) {
      delay(500);
      Serial.print(".");
      timeout++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Reconnected to WiFi");
    } else {
      Serial.println("Failed to reconnect to WiFi");
    }
  }
  
  lastWeatherUpdate = millis();
}

void parseWeatherData(String json) {
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, json);
  
  if (error) {
    Serial.print("JSON parsing failed: ");
    Serial.println(error.c_str());
    return;
  }
  
  currentWeather.temperature = doc["main"]["temp"];
  currentWeather.feelsLike = doc["main"]["feels_like"];
  currentWeather.humidity = doc["main"]["humidity"];
  currentWeather.windSpeed = doc["wind"]["speed"];
  currentWeather.weatherDescription = doc["weather"][0]["description"].as<String>();
  currentWeather.weatherIcon = doc["weather"][0]["icon"].as<String>();
  currentWeather.lastUpdate = millis();
  
  Serial.println("Weather data updated");
}

void handleLocationChange() {
  if (server.hasArg("city") && server.hasArg("country")) {
    currentCity = server.arg("city");
    currentCountryCode = server.arg("country");
    
    // Fetch weather for new location
    fetchWeatherData();
    
    // Redirect back to root
    server.sendHeader("Location", "/");
    server.send(303);
  } else {
    server.send(400, "text/plain", "Bad Request - Missing parameters");
  }
}

void handleResetLocation() {
  currentCity = defaultCity;
  currentCountryCode = defaultCountryCode;
  
  // Fetch weather for default location
  fetchWeatherData();
  
  // Redirect back to root
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleWiFiReset() {
  server.send(200, "text/html", 
    "<html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'>"
    "<title>Resetting WiFi...</title>"
    "<style>body{font-family:Arial,sans-serif;text-align:center;padding:20px;}</style>"
    "</head><body>"
    "<h2>WiFi Settings Reset</h2>"
    "<p>WiFi credentials have been reset. The device will restart and create an access point named 'ESP-Weather-Setup'.</p>"
    "<p>Connect to this network to configure new WiFi settings.</p>"
    "</body></html>");

  // Give time for the response to be sent
  delay(2000);
  
  // Reset WiFi settings
  WiFiManager wifiManager;
  wifiManager.resetSettings();
  
  // Restart the ESP
  ESP.restart();
}

void handleRoot() {
  String locationStatus = "";
  if (!currentWeather.dataValid) {
    locationStatus = "<div class='error-message'>Unable to fetch weather data for " + 
                     currentCity + ". Please check the location.</div>";
  }
  
  String currentLocationInfo = "";
  if (currentCity != defaultCity || currentCountryCode != defaultCountryCode) {
    currentLocationInfo = "<div class='custom-location'>Showing weather for: <b>" + 
                         currentCity + ", " + currentCountryCode + 
                         "</b> <a href='/resetLocation'>[Reset to default]</a></div>";
  }

  String html = "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                "<title>ESP8266 Weather Station</title>"
                "<link rel='stylesheet' type='text/css' href='/style.css'>"
                "<script>"
                "function refreshWeather() {"
                "  fetch('/weather')"
                "    .then(response => response.json())"
                "    .then(data => {"
                "      document.getElementById('temperature').innerText = data.temperature + '°C';"
                "      document.getElementById('feels-like').innerText = data.feelsLike + '°C';"
                "      document.getElementById('humidity').innerText = data.humidity + '%';"
                "      document.getElementById('wind-speed').innerText = data.windSpeed + ' m/s';"
                "      document.getElementById('description').innerText = data.description;"
                "      document.getElementById('weather-icon').src = 'http://openweathermap.org/img/wn/' + data.icon + '@2x.png';"
                "      document.getElementById('last-update').innerText = 'Last updated: ' + new Date().toLocaleTimeString();"
                "      document.getElementById('connection-status').innerText = '';"
                "    })"
                "    .catch(error => {"
                "      console.error('Error fetching weather data:', error);"
                "      document.getElementById('connection-status').innerText = 'Connection error. Retrying...';"
                "    });"
                "}"
                "setInterval(refreshWeather, 10000);"  // Refresh every 10 seconds
                "document.addEventListener('DOMContentLoaded', refreshWeather);"
                "</script>"
                "</head>"
                "<body>"
                "<div class='container'>"
                "<h1>ESP8266 Weather Station</h1>"
                "<div class='location'>" + currentCity + ", " + currentCountryCode + "</div>" +
                locationStatus +
                currentLocationInfo +
                "<div class='connection-status' id='connection-status'></div>"
                "<div class='weather-container'>"
                "<div class='weather-icon'><img id='weather-icon' src=''></div>"
                "<div class='weather-info'>"
                "<div class='temperature' id='temperature'>--°C</div>"
                "<div class='description' id='description'>--</div>"
                "</div>"
                "</div>"
                "<div class='details'>"
                "<div class='detail'><span class='label'>Feels Like:</span> <span id='feels-like'>--°C</span></div>"
                "<div class='detail'><span class='label'>Humidity:</span> <span id='humidity'>--%</span></div>"
                "<div class='detail'><span class='label'>Wind Speed:</span> <span id='wind-speed'>-- m/s</span></div>"
                "</div>"
                "<div class='search-container'>"
                "<form action='/changeLocation' method='post'>"
                "<h3>Check Weather for Another Location</h3>"
                "<div class='form-group'>"
                "<label for='city'>City:</label>"
                "<input type='text' id='city' name='city' placeholder='Enter city name' required>"
                "</div>"
                "<div class='form-group'>"
                "<label for='country'>Country Code:</label>"
                "<input type='text' id='country' name='country' placeholder='Country code (e.g., IN, US)' required maxlength='2'>"
                "</div>"
                "<button type='submit'>Get Weather</button>"
                "</form>"
                "</div>"
                "<div class='device-info'>"
                "<div>Device IP: " + WiFi.localIP().toString() + "</div>"
                "<div>WiFi: " + String(WiFi.SSID()) + "</div>"
                "</div>"
                "<div class='last-update' id='last-update'>Last updated: --</div>"
                "<button onclick='refreshWeather()'>Refresh Now</button>"
                "<div class='wifi-reset'>"
                "<hr>"
                "<h3>WiFi Settings</h3>"
                "<p class='warning'>Warning: Clicking the button below will reset your WiFi settings and restart the device.</p>"
                "<button class='danger-btn' onclick=\"if(confirm('Are you sure you want to reset WiFi settings? This will restart the device.')) { window.location.href='/resetWiFi'; }\">Reset WiFi Settings</button>"
                "</div>"
                "</div>"
                "</body>"
                "</html>";
  server.send(200, "text/html", html);
}

void handleRefresh() {
  fetchWeatherData();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleCSS() {
  String css = "* { box-sizing: border-box; margin: 0; padding: 0; }"
               "body { font-family: Arial, sans-serif; background-color: #f5f5f5; color: #333; }"
               ".container { max-width: 600px; margin: 0 auto; padding: 20px; background-color: white; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); margin-top: 20px; margin-bottom: 20px; }"
               "h1 { text-align: center; color: #0066cc; margin-bottom: 10px; }"
               "h3 { margin-bottom: 15px; color: #333; }"
               ".location { text-align: center; font-size: 1.2em; margin-bottom: 10px; color: #666; }"
               ".custom-location { text-align: center; font-size: 0.9em; margin-bottom: 15px; background-color: #f0f8ff; padding: 8px; border-radius: 5px; }"
               ".custom-location a { text-decoration: none; color: #0066cc; }"
               ".error-message { text-align: center; color: #cc0000; margin-bottom: 10px; font-weight: bold; background-color: #ffeeee; padding: 8px; border-radius: 5px; }"
               ".connection-status { text-align: center; color: #ff6600; margin-bottom: 10px; font-weight: bold; }"
               ".weather-container { display: flex; align-items: center; justify-content: center; margin-bottom: 20px; }"
               ".weather-icon img { width: 100px; height: 100px; }"
               ".weather-info { margin-left: 20px; }"
               ".temperature { font-size: 3em; font-weight: bold; }"
               ".description { font-size: 1.2em; text-transform: capitalize; color: #666; }"
               ".details { background-color: #f9f9f9; padding: 15px; border-radius: 8px; margin-bottom: 20px; }"
               ".detail { margin-bottom: 8px; }"
               ".search-container { background-color: #f0f8ff; padding: 15px; border-radius: 8px; margin-bottom: 20px; }"
               ".form-group { margin-bottom: 12px; }"
               ".form-group label { display: block; margin-bottom: 5px; font-weight: bold; }"
               ".form-group input { width: 100%; padding: 8px; border: 1px solid #ddd; border-radius: 4px; }"
               ".device-info { font-size: 0.9em; color: #999; background-color: #f9f9f9; padding: 10px; border-radius: 8px; margin-bottom: 15px; }"
               ".label { font-weight: bold; }"
               ".last-update { text-align: center; color: #999; margin-bottom: 15px; font-size: 0.9em; }"
               "button { display: block; margin: 0 auto; padding: 10px 20px; background-color: #0066cc; color: white; border: none; border-radius: 5px; cursor: pointer; font-size: 1em; }"
               "button:hover { background-color: #0055aa; }"
               ".wifi-reset { margin-top: 20px; background-color: #fff0f0; padding: 15px; border-radius: 8px; }"
               ".warning { color: #cc0000; font-size: 0.9em; margin-bottom: 10px; }"
               ".danger-btn { background-color: #cc0000; margin-top: 10px; }"
               ".danger-btn:hover { background-color: #aa0000; }"
               "hr { margin: 20px 0; border: none; height: 1px; background-color: #ddd; }";
  server.send(200, "text/css", css);
}

void handleWeatherData() {
  if (currentWeather.dataValid) {
    String json = "{\"temperature\":" + String(currentWeather.temperature, 1) + 
                  ",\"feelsLike\":" + String(currentWeather.feelsLike, 1) + 
                  ",\"humidity\":" + String(currentWeather.humidity) + 
                  ",\"windSpeed\":" + String(currentWeather.windSpeed, 1) + 
                  ",\"description\":\"" + currentWeather.weatherDescription + "\"" +
                  ",\"icon\":\"" + currentWeather.weatherIcon + "\"}";
    server.send(200, "application/json", json);
  } else {
    server.send(503, "application/json", "{\"error\":\"Weather data not available\"}");
  }
}
