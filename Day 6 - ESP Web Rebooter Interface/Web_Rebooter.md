#  Day 6 – ESP Web Rebooter Interface

Give your ESP8266/ESP32 the ability to **soft-restart (ESP.restart()) via a secure web interface**.  
No physical reset needed — just log in, click, and reboot remotely!

---

##  Concept

Sometimes your ESP device needs to be restarted after a config change, memory leak, or OTA.  
This project provides a secure **web-based reboot panel** served from the ESP itself.

---

##  Hardware Required

- ✅ ESP8266 or ESP32
- ✅ Connected to WiFi
- ❌ No extra sensors or buttons needed

---

##  Features

| Feature              | Description                                 |
|----------------------|---------------------------------------------|
| Web reboot page       | `/reboot` endpoint with confirmation button |
| Password protection   | Basic PIN/token authentication (optional)   |
| ESP soft restart      | Triggers `ESP.restart()` on confirmation    |


---

## 🔩 Key Functions Used

```cpp
ESP.restart();             // Soft-reboot the device
AsyncWebServer for web UI
HTTP GET for triggering
```

