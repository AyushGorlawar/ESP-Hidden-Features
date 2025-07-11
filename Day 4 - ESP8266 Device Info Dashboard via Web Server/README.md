# Day 4 – ESP Device Info Dashboard via Web Server

Build a live **dashboard** served directly from your ESP8266/ESP32 that shows **real-time system stats** like WiFi strength, uptime, heap memory, flash size, and more — all accessible via any browser!

---

##  Concept

This project transforms your ESP board into a **monitoring web server**, offering a compact and lightweight dashboard UI.

Perfect for:
- 📡 Monitoring deployed devices
- 🛠 Debugging firmware issues without serial
- 📱 Accessing device status over phone or PC

---

##  Hardware Required

-  ESP8266 (NodeMCU) or ESP32
-  WiFi access
-  No extra sensors required

---

##  Dashboard Features

| Metric                | Value Example              |
|-----------------------|----------------------------|
| 📶 WiFi SSID          | MyHomeNetwork              |
| 📡 RSSI               | -53 dBm                    |
| 🌐 IP Address         | 192.168.1.45               |
| 🔁 Uptime             | 2 mins 34 secs             |
| 💾 Heap Memory        | 40128 bytes                |
| ⚙️ Chip ID            | 0x83DE79                   |
| 🔧 Flash Size         | 4 MB                       |

🌀 Dashboard auto-refreshes every 5–10 seconds using JavaScript or meta refresh.


---

## 🔩 Key Functions Used

```cpp
WiFi.SSID()
WiFi.RSSI()
WiFi.localIP()
ESP.getChipId()
ESP.getFreeHeap()
millis() → for uptime
```


