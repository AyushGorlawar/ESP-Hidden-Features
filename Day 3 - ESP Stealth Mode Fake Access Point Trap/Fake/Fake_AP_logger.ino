// Day 3 – ESP Fake Access Point Honeypot Logger
// Logs MAC addresses of devices connecting to ESP's fake WiFi

#include <ESP8266WiFi.h>
#include <user_interface.h>

struct station_info *station_list;

const char *ssid = "Free_Public_WiFi"; // Fake open WiFi name

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  Serial.println("[INFO] Fake WiFi Access Point Created");
  Serial.print("[SSID] "); Serial.println(ssid);
  Serial.print("[IP] "); Serial.println(WiFi.softAPIP());
}

void loop() {
  station_list = wifi_softap_get_station_info();
  Serial.println("\n[+] Connected Devices:");
  while (station_list != NULL) {
    Serial.print("MAC: ");
    for (int i = 0; i < 6; i++) {
      if (station_list->bssid[i] < 16) Serial.print("0");
      Serial.print(station_list->bssid[i], HEX);
      if (i < 5) Serial.print(":");
    }
    Serial.println();
    station_list = STAILQ_NEXT(station_list, next);
  }
  wifi_softap_free_station_info();
  delay(5000); // Check every 5 seconds
}
