// Works with both ESP8266 and ESP32
#include <ESP8266WiFi.h> // For ESP32, use: #include <WiFi.h>

// Optional alert output
#define ALERT_LED D1  // Use any GPIO pin
//#define BUZZER D2   // Uncomment if using buzzer

void setup() {
  Serial.begin(115200);
  delay(10);

  // Set alert pin
  pinMode(ALERT_LED, OUTPUT);
  digitalWrite(ALERT_LED, LOW);

  // Start in promiscuous (monitor) mode
  WiFi.mode(WIFI_OFF);
  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(0);
  wifi_set_promiscuous_rx_cb(promiscuousCallback);
  wifi_promiscuous_enable(1);

  Serial.println("\n[+] ESP is now monitoring WiFi packets...");
}

void loop() {
  // Nothing here – callback handles everything
}

// Raw 802.11 packet structure
typedef struct {
  unsigned frame_ctrl:16;
  unsigned duration_id:16;
  uint8_t addr1[6];   // Destination
  uint8_t addr2[6];   // Source (attacker MAC)
  uint8_t addr3[6];   // BSSID
} wifi_hdr_t;

void promiscuousCallback(uint8_t *buf, uint16_t len) {
  if (len == 0) return;

  wifi_hdr_t *hdr = (wifi_hdr_t*) buf;
  uint8_t frameType = buf[0];
  uint8_t frameSubtype = buf[0] & 0xF0;

  // Check for Deauthentication frame (Type: Mgmt, Subtype: 0xC0)
  if (frameType == 0xC0) {
    Serial.println("\n[!] Deauthentication Packet Detected!");

    Serial.print(" > Attacker MAC: ");
    printMAC(hdr->addr2);

    digitalWrite(ALERT_LED, HIGH);
    delay(500);
    digitalWrite(ALERT_LED, LOW);
  }
}

void printMAC(const uint8_t *mac) {
  for (int i = 0; i < 6; i++) {
    if (mac[i] < 16) Serial.print("0");
    Serial.print(mac[i], HEX);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
}
