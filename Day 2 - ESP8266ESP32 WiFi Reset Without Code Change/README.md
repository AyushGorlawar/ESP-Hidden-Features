# Day 2 – ESP WiFi Reset Without Code Change

Give your ESP8266/ESP32 the ability to **change WiFi credentials at runtime** without editing the code or re-uploading via Arduino IDE.

---

## Concept

This project solves a common IoT pain point — when WiFi credentials change (new router, SSID/password updated), we usually have to reflash the ESP. Not anymore.

With this method:
- ESP boots and tries saved credentials
- If it fails, it switches to **Access Point (AP)** mode
- You connect to its hotspot and visit `192.168.4.1`
- Enter your new SSID + Password via a simple web form
- ESP saves it to **LittleFS** or **EEPROM**
- Reboots and connects to the new WiFi

---

## ⚙ Hardware Required

- ✅ ESP8266 (NodeMCU/Wemos D1 mini) or ESP32
- ✅ USB cable + Arduino IDE
- ❌ No sensors or external modules required

---

## 🌐 How It Works

1. **ESP boots → tries to connect to saved WiFi**
2. If connection fails within timeout, it:
   - Starts AP mode (e.g., SSID: `ESP_Config`)
   - Launches captive portal / web page
3. User connects and opens browser to `192.168.4.1`
4. HTML form appears asking for SSID + password
5. On submit:
   - Credentials are saved in LittleFS
   - ESP reboots and connects to new network

---

##  Preview

![Web Config UI Preview](https://your-image-url-if-available)

---

## Storage Method

You can choose between:
- **LittleFS (preferred)** – stores data as a file
- **EEPROM** – for small setups or minimal storage

This example uses **LittleFS**, but can be adapted for EEPROM by replacing the file operations with EEPROM read/write logic.

---

##  Security Note

- Add password to your AP mode (`WiFi.softAP("ESP_Config", "yourpass")`) to prevent open access
- Add validation in HTML to avoid accidental empty credentials

---

##  Why This Is Powerful

- Ideal for **commercial IoT products**
- Allows **non-tech users** to set up WiFi easily
- Works without needing OTA or serial access
- **No hardcoding** of WiFi credentials

---

## Author

Built with ❤️ by [Ayush Gorlawar](https://github.com/AyushGorlawar)  
Part of the [ESP-Hidden-Features](https://github.com/AyushGorlawar/ESP-Hidden-Features) series 



