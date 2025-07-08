# ESP8266 Wi-Fi Honeypot

This project sets up a fake Wi-Fi access point using an ESP8266 module, mimicking a public Wi-Fi login page to capture user details. The captured data is sent to Firebase Realtime Database for analysis. **This project is strictly for educational and cybersecurity awareness purposes.**

## Features
- Creates an open Wi-Fi hotspot named **RailWire**.
- Hosts a fake **RailWire Free Wi-Fi Login Page**.
- Captures email, password, device screen size, IP address, MAC address, and user agent.
- Stores the captured data in **Firebase Realtime Database**.

## Prerequisites
- **ESP8266 Module** (NodeMCU, Wemos D1 Mini, etc.)
- **Arduino IDE** with ESP8266 Board support installed
- **Firebase Realtime Database** setup with API credentials

## Installation & Setup
### 1. Install Dependencies
Ensure you have the following libraries installed in **Arduino IDE**:
- `ESP8266WiFi.h`
- `DNSServer.h`
- `ESP8266WebServer.h`
- `FirebaseESP8266.h`

### 2. Configure Firebase
Update the following credentials in the code:
```cpp
#define FIREBASE_HOST "esp-honeypot-default-rtdb.firebaseio.com"  
#define FIREBASE_AUTH "YOUR_FIREBASE_SECRET_KEY"
```
Replace `YOUR_FIREBASE_SECRET_KEY` with your actual Firebase authentication key.

### 3. Flash the Code to ESP8266
Upload the provided `.ino` file to your ESP8266 board using **Arduino IDE**.

## Usage
1. **Power on** the ESP8266 module.
2. A new Wi-Fi network **RailWire** will appear.
3. When a user connects and enters details, the data is captured and stored in Firebase.
4. View the stored data at:
   ```
   your realtime databse \user.json
   ```

## Firebase Security Rules
If you want to access data without authentication, modify Firebase **Realtime Database Rules**:
```json
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```
> ⚠️ **Warning:** Enabling public access makes your database vulnerable.

## Disclaimer
**This project is strictly for educational and cybersecurity awareness purposes.** Unauthorized use of this project for malicious activities is illegal and punishable under cyber laws. The author is not responsible for any misuse of this code..

## License
This project is released under the **Apache License 2.0**.
