#  Day 10 – Hidden UART Debug Mode on ESP8266

##  Feature: Internal Debug Log via UART0 @ 74880 baud

Most developers use Serial at 9600 or 115200, but the ESP8266 has a **hidden debug log channel** that transmits **boot logs, crash traces, and system info** at **74880 baud** on **TX (GPIO1)**.

This feature is especially useful when your ESP8266 fails to boot or crashes before `setup()` is even reached.

---

##  Requirements

- ESP8266 board (NodeMCU, Wemos D1 Mini, etc.)
- USB to TTL converter (for separate UART log capture)
- Serial Terminal (Arduino Serial Monitor, PuTTY, TeraTerm, etc.)
- Optional: SD card module or Arduino for data logging

---

### Wiring

| ESP8266 Pin | Connects To           |
|-------------|------------------------|
| GPIO1 (TX)  | USB-TTL RX (direct)    |
| GND         | USB-TTL GND            |

> ⚠️ Do not use GPIO1 for other Serial.print if you want clean logs.

---

##  Serial Terminal Settings

- **Baud rate**: 74880
- **Data Bits**: 8
- **Parity**: None
- **Stop Bits**: 1

---


##  What You'll See

- Bootloader messages
- WDT resets
- Flash loading status
- Crash reasons (illegal instructions, heap corruption)
- WiFi stack internal logs (if enabled)

---

##  Use-Cases

- Debugging boot issues when normal Serial fails
- Capturing rare crashes or OTA failures
- Diagnosing flash or reset bugs
- Monitoring ESP at the **bare-metal** level

---

## Advanced: Log to SD Card or Secondary MCU

You can also forward this output to:
- A **microSD card** using SoftwareSerial
- An **Arduino Uno** for offline logging
- A **Raspberry Pi** as a monitoring backend

> Let us know if you want a logging implementation!

---

##  Fun Fact

Only ~10% of devs ever explore the **74880 debug UART mode** — and it’s a life-saver during hardware-level bugs.

---

##  Disclaimer

This is a **read-only serial feature**; you cannot send data **to** ESP8266 at this baud. Use it only for passive monitoring or forensic debugging.

---

### Stay tuned for BONUS features or Day 11 surprise drops!



