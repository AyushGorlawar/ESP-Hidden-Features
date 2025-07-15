#  Day 7 – ESP Hidden Command Listener (Web Backdoor Console)

Create a **secret, UI-less command portal** for your ESP8266/ESP32 that executes specific actions (like reboot, LED control, WiFi reset, format, etc.) based on hidden GET requests.

No web form. No interface. Just smart, silent triggers via browser.

---

##  Concept

Useful during:
- Field deployments
- Debugging without serial monitor
- Hidden maintenance features
- Secure dev-only operations

Example:
```
http://192.168.1.42/cmd?token=ayush007&action=led_on

```

---

##  Hardware Required

- ✅ ESP8266/ESP32 board
- ✅ Onboard LED (GPIO2 for NodeMCU)
- ❌ No sensors needed

---

##  Supported Hidden Commands

| Command      | Action Triggered                          |
|--------------|-------------------------------------------|
| `status`     | Returns uptime, heap memory, IP           |
| `led_on`     | Turns onboard LED ON                      |
| `led_off`    | Turns onboard LED OFF                     |
| `reboot`     | Soft resets ESP                           |
| `reset_wifi` | Clears WiFi settings (ESP.eraseConfig)    |
| `format_fs`  | Formats LittleFS (⚠️ erase all files)      |

---

##  Security

- Access token required in URL (e.g., `?token=ayush007`)
- Invalid or missing tokens get blocked with 403 response


---

##  Core Functions Used

```cpp
ESP.restart();
ESP.eraseConfig();
LittleFS.format();
digitalWrite(LED_BUILTIN, HIGH/LOW);
millis(), WiFi.localIP(), ESP.getFreeHeap();
```

## Limitations
- Max buffer ~10–20 messages in memory (ESP RAM limit)

- No user identity unless implemented

- No message persistence after reboot (unless stored in LittleFS)
