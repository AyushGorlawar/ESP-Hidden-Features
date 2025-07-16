#  Day 8 – ESP8266 WiFi Beacon Spammer (Fake SSID Broadcaster)

Turn your ESP8266 into a **WiFi network illusionist** — broadcast dozens of fake SSIDs visible on any nearby phone or laptop.  
This trick uses low-level packet injection to simulate access points without actually creating them.

---

##  What Makes It Hidden?

- Uses the rare `wifi_send_pkt_freedom()` function from the **ESP8266 SDK**
- Doesn't create real access points — just beacon frames
- Not officially documented in Arduino core
- Works *even without hosting WiFi* — it's pure signal manipulation

---

##  Use Cases

| Use Case                   | Description                                     |
|----------------------------|-------------------------------------------------|
| WiFi scanner testing       | Flood SSIDs to check scanner detection          |
| Hacking/CTF demos          | Show spoofed SSIDs like `Free_Hack_Network`     |
| Funny pranks               | Display “FBI Van 🚔” or “Free Pizza WiFi”       |
| Captive portal decoy test  | Fake hotspot simulation                         |

---

##  Legal Warning

This is **for education only**.

❌ Do **NOT** use in public places  
❌ Do **NOT** target real networks  
✅ Only use in isolated or shielded environments

---

##  Hardware Required

- ✅ ESP8266 Board (e.g., NodeMCU, Wemos D1 mini)
- ❌ ESP32 NOT SUPPORTED (as `wifi_send_pkt_freedom` is ESP8266-specific)

---

## Setup Instructions

1. Open Arduino IDE
2. Install ESP8266 board package
3. Enable `#include <user_interface.h>` (may require adding it manually)
4. Upload `beacon_spammer.ino`
5. Open Serial Monitor for logs

---

##  Customization

Change the `ssidList[]` array to broadcast your own set of names:
