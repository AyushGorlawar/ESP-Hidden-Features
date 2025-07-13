#  Day 5 – ESP Realtime Chat Server

Transform your ESP8266/ESP32 into a **local web-based chat server** where multiple users connected to the same WiFi can chat in real time — **without any internet or cloud**.

---

##  Concept

This project demonstrates how an ESP can host a **LAN-only chat room** using:
- A web server
- AJAX-based messaging (or WebSockets for advanced versions)
- Temporary in-memory storage (like a message buffer)

---

##  Hardware Required

- ✅ ESP8266 (NodeMCU) or ESP32
- ✅ Local WiFi or ESP as Access Point
- ✅ No database, no cloud — works fully offline!

---

##  Features

| Feature              | Description                          |
|----------------------|--------------------------------------|
| Web chat interface   | Mobile & desktop friendly             |
| Message sending      | Uses AJAX (or WebSocket) calls       |
| Chat history buffer  | Stored in device memory (limited)    |
| Time + name tag      | Optional sender name or timestamp     |


---

##  Key Libraries

```cpp
ESP8266WiFi.h
ESPAsyncWebServer.h
ESPAsyncTCP.h
```
