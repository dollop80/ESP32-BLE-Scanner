# ESP32-BLE-Scanner
The ESP32-BLE-Scanner is used to detect BLE devices and to read RSSI value for each.

# Detailed description
The BLE-Scanner has a list of known devices (pre-defined by user) and a RSSI threshold. Once the list of detected devices contains any of known devices and its RSSI is greater than the threshold, the BLE-scanner turns on a LED. When there no detected devices from the list and with RSSI signal greater than threshold for 3 minutes the LED turns off.

# Preparation
*Hardware*
- Any ESP32 module or development board. For debugging purposes the [TTGO-T8-ESP32](https://github.com/LilyGO/TTGO-T8-ESP32) board was used

*Software*
- Arduino IDE (1.8 or above)
- Arduino IDE ESP32 Board Installation

# Installation and Running

1. Open the ESP32_BLE_Scanner.ino Arduino sketch in Arduino IDE.
2. Edit defines to fit your needs:

```
#define LED_PIN 21                // Pin for LED to indicate beacon presence
#define MIN_RSSI -80              // Minimum RSSI [db] needed to perform address comparison
#define SCAN_TIME 10              // seconds
#define LOOP_TIME 5               // seconds
#define NO_DEVICE_TIMER 3*60      // seconds
```

3. Compile ESP32_BLE_Scanner.ino Arduino sketch, and flash it to ESP32 board. After flashing, ESP32 scans for BLE devices during SCAN_TIME and behaves as listed in [Detailed description](# Detailed description). You can check what the scanner does using Arduino terminal
