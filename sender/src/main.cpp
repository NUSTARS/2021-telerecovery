/*

  Adapted from OLED_LoRa_Sender

*/
#include <Arduino.h>
#include "heltec.h"
#include "images.h"
#include "ToF.h"

#define BAND 915E6  // You can set band here directly,e.g. 868E6,915E6


Adafruit_VL53L0X ToF = Adafruit_VL53L0X();

unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;

void setup(){
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 
  delay(1500);


  Serial.begin(115200);
    
  // Wait until serial port opens for native USB devices
  while (! Serial) {
      delay(1);
  }

  // Initialize time of flight sensor
  if (!ToF.begin()) {
      Serial.println(F("Failed to boot VL53L0X"));
      while(1);
  }

}

void loop(){

  VL53L0X_RangingMeasurementData_t val = read_ToF(ToF);

  if (val.RangeStatus != 4) {  // phase failures have incorrect data
      Serial.print("Distance (mm): "); 
      Serial.println(val.RangeMilliMeter);
  } 
  else {
      Serial.println(" Out of range ");
  }

  // send packet
  LoRa.beginPacket();
  
// /*
//  * LoRa.setTxPower(txPower,RFOUT_pin);
//  * txPower -- 0 ~ 20
//  * RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
//  *   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
//  *   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
// */
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print("hello");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
  delay(1000);

}
