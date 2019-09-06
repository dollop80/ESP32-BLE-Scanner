/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
*/

#include <Arduino.h>
#include <sstream>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

//*************************************************
// Defines
//*************************************************
#define LED_PIN 21                // Pin for LED to indicate beacon presence
#define MIN_RSSI -80              // Minimum RSSI [db] needed to perform address comparison
#define SCAN_TIME 10              // seconds
#define LOOP_TIME 5               // seconds
#define NO_DEVICE_TIMER 1*60      // seconds

// comment the follow line to disable serial message
#define SERIAL_PRINT


//*************************************************
// Global Variables
//*************************************************
//Device Address List. Include a devices' address 
String addrList[] = {"90:dd:5d:a2:a2:80", "18:4f:32:46:42:2a", "00:9e:c8:ae:d8:c1", "25:db:53:34:04:ce", " 0c:d1:5b:d9:c3:e2"};

long noDevicesMillis = 0;
bool present = false;
bool scanFinished = true;


//*************************************************
// BLEAdvertisedDevice Callback
//*************************************************
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
#ifdef SERIAL_PRINT
      Serial.printf("Advertised Device: %s, RSSI: %d \n", advertisedDevice.getAddress().toString().c_str(), advertisedDevice.getRSSI());
#endif
    }
};

//*************************************************
// Setup function. Runs once
//*************************************************
void setup()
{
  pinMode(LED_PIN, OUTPUT);                   //GPIO output for LED
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  //disable brownout detector

#ifdef SERIAL_PRINT
  Serial.begin(115200);
  Serial.println("ESP32 BLE Scanner");
#endif

}

//*************************************************
// Loop function. Runs forever
//*************************************************
void loop()
{
  if(scanFinished)
  {
    scanFinished = false;
    performScan();
  }
  
  if(!present)
  {
    if(millis() > noDevicesMillis + NO_DEVICE_TIMER * 1000)
    {
      digitalWrite(LED_PIN, LOW);
    }
  }
  else
  {
    digitalWrite(LED_PIN, HIGH);
    noDevicesMillis = millis();
  }
}

//*************************************************
// BLE devices scan. Here ve decide if a davice belongs to list
//*************************************************
void performScan()
{
  BLEDevice::init("");

  BLEScan *pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(0x50);
  pBLEScan->setWindow(0x30);

#ifdef SERIAL_PRINT
  Serial.printf("Start BLE scan for %d seconds...\n", SCAN_TIME);
#endif

  BLEScanResults foundDevices = pBLEScan->start(SCAN_TIME);
  int count = foundDevices.getCount();
  present = false;
  for (int i = 0; i < count; i++)
  {  
    BLEAdvertisedDevice d = foundDevices.getDevice(i);
    String currDevAddr = d.getAddress().toString().c_str();
    for(int j = 0; j < sizeof(addrList)/sizeof(addrList[0]); j++)
    {
      if(d.getRSSI() >= MIN_RSSI && addrList[j].equalsIgnoreCase(currDevAddr))
      {
        present = true;
        break;
      }
    }
  }
  
#ifdef SERIAL_PRINT
  Serial.println("Scan done!");
#endif
  scanFinished = true;
}
