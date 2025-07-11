#  Day 3 – ESP Stealth Mode: Fake Access Point Trap

Turn your ESP8266/ESP32 into a **WiFi honeypot** (fake access point) that silently logs all connection attempts — great for ethical hacking, research, or learning about wireless security.

---

##  Concept

This sketch creates a **fake open WiFi network** (e.g., `Free_WiFi`, `Cafe_WiFi`) using ESP’s Access Point mode.  
Any nearby device trying to auto-connect or manually clicking on this network will be **logged** with its:

- 📡 MAC address
- ⏱ Timestamp of connection
- 📱 Optional: User-Agent or Device Type via HTTP request headers

---

##  Hardware Required

- ✅ ESP8266 or ESP32
- ✅ No sensors or internet required
- ✅ Works standalone with just power + serial monitor

---


## 💻 How It Works

1. ESP starts in **softAP** mode (no password, open WiFi)
2. Client devices see an open network and attempt to connect
3. ESP logs:
   - MAC address of connected devices
   - Total number of connected devices
   - Optional HTTP headers if they visit the portal

---

## 🔩 Key Functions Used

```cpp
WiFi.softAP(ssid);                          // Creates the fake WiFi network
wifi_softap_get_station_info();            // Gets info about connected stations
```

