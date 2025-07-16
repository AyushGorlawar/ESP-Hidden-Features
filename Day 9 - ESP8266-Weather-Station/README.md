# ESP8266 Weather Station

## Overview
This project is a **Weather Station using ESP8266** that fetches real-time weather data from the **OpenWeather API** and hosts a web server to display the data.

## Features
- Fetches live weather data (temperature, humidity, wind speed, etc.)
- Serves a web page via ESP8266 to display the weather details
- Auto-refreshes weather information periodically
- Simple and lightweight design

## Hardware Requirements
- **ESP8266 NodeMCU**
- **Micro USB cable** (for power and programming)
- **Wi-Fi connection**

## Software Requirements
- **Arduino IDE** (with ESP8266 board support installed)
- **Libraries Required:**
  - `ESP8266WiFi.h` (for WiFi connectivity)
  - `ESP8266HTTPClient.h` (for HTTP requests)
  - `ArduinoJson.h` (for JSON parsing)

## Installation & Setup
1. **Install Arduino IDE** and add ESP8266 board support.
2. **Install required libraries** from Library Manager.
3. **Get OpenWeather API Key:**
   - Sign up at [OpenWeather](https://openweathermap.org/api) and get your free API key.
4. **Modify `ssid` and `password` in the code** to match your Wi-Fi credentials.
5. **Replace `YOUR_API_KEY` in the code** with your OpenWeather API key.
6. **Upload the code** to ESP8266 using Arduino IDE.

## How It Works
1. ESP8266 connects to **Wi-Fi**.
2. It fetches weather data using the **OpenWeather API**.
3. The data is parsed and stored.
4. ESP8266 hosts a **webpage** to display the weather details.
5. Users can access it via a **browser** using ESP8266's IP.

## Accessing the Webpage
- After uploading the code, open the **Serial Monitor**.
- ESP8266 will print its **local IP address**.
- Open a browser and enter the IP address to see the weather details.

## Troubleshooting
- **Wi-Fi Not Connecting?**
  - Double-check the `ssid` and `password`.
  - Ensure your router is working.
- **Webpage Not Loading?**
  - Check if ESP8266 is powered and connected to Wi-Fi.
  - Reboot the module.
- **API Key Error?**
  - Ensure you have entered a valid OpenWeather API key.
  - Some free-tier API keys have request limits.

## Future Improvements
- Add OLED or LCD display support
- Implement auto-refreshing without page reload
- Use deep sleep for power optimization



