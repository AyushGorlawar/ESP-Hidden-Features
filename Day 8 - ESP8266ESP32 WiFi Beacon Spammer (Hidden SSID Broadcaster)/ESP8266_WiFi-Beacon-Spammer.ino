// Day 8 – ESP8266 WiFi Beacon Spammer
// WARNING: For education only. Don't use in public networks.

#include <ESP8266WiFi.h>
extern "C" {
  #include "user_interface.h"
}

const char* ssidList[] = {
  "Free_WiFi_Ayush",
  "🔐 Hack_Me_If_U_Can",
  "FBI_Surveillance_Van",
  "No_Internet_For_You",
  "💀 Virus_Hub_69",
  "ESP_Hidden_Trap",
  "AyushNet",
  "404_WiFi_NotFound"
};

const uint8_t beaconPacket[] = {
  0x80, 0x00, // Beacon frame
  0x00, 0x00, // Duration
  // MACs will be inserted here dynamically
};

uint8_t fakeBeacon[128];
uint8_t macTemplate[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x00};

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  wifi_promiscuous_enable(0);
  wifi_set_opmode(STATION_MODE);
  wifi_set_channel(6);  // Stable common channel
  wifi_promiscuous_enable(1);
  delay(2000);
  Serial.println("\n[Beacon Spammer] Broadcasting fake SSIDs...");
}

void loop() {
  for (int i = 0; i < sizeof(ssidList)/sizeof(ssidList[0]); i++) {
    int len = createBeaconPacket(ssidList[i]);
    bool success = wifi_send_pkt_freedom(fakeBeacon, len, 0) == 0;
    Serial.printf("Sent: %s [%s]\n", ssidList[i], success ? "OK" : "FAIL");
    delay(100);
  }
  delay(1000);
}

int createBeaconPacket(const char* ssid) {
  int ssidLen = strlen(ssid);
  int packetLen = 0;

  // Reset packet
  memset(fakeBeacon, 0, sizeof(fakeBeacon));

  // Beacon Frame Header
  fakeBeacon[0] = 0x80;
  fakeBeacon[1] = 0x00;
  fakeBeacon[2] = 0x00;
  fakeBeacon[3] = 0x00;
  packetLen = 4;

  // MACs (destination, source, BSSID)
  for (int j = 0; j < 3; j++) {
    macTemplate[5] = i + j;
    memcpy(&fakeBeacon[packetLen], macTemplate, 6);
    packetLen += 6;
  }

  // Sequence Control
  fakeBeacon[packetLen++] = 0x00;
  fakeBeacon[packetLen++] = 0x00;

  // Timestamp + Interval + Capabilities
  for (int j = 0; j < 8 + 2 + 2; j++) fakeBeacon[packetLen++] = 0x00;

  // SSID Tag
  fakeBeacon[packetLen++] = 0x00;
  fakeBeacon[packetLen++] = ssidLen;
  memcpy(&fakeBeacon[packetLen], ssid, ssidLen);
  packetLen += ssidLen;

  // Supported Rates Tag (fake 1Mbps)
  fakeBeacon[packetLen++] = 0x01;
  fakeBeacon[packetLen++] = 0x01;
  fakeBeacon[packetLen++] = 0x82;

  return packetLen;
}
