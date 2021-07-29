/*
  Adapted from ...\examples\LoRa\OLED_LoRa_Sender.ino
*/
#include <Arduino.h>
#include "heltec.h"
#include "ToF.h"

#define BAND 915E6  // You can set band here directly,e.g. 868E6,915E6

Adafruit_VL53L0X ToF = Adafruit_VL53L0X();

void setup(){

  Heltec.begin(
    false /*DisplayEnable Enable*/,
    true /*Heltec.Heltec.Heltec.LoRa Disable*/, 
    true /*Serial Enable*/, 
    true /*PABOOST Enable*/, 
    BAND /*long BAND*/);
 
  delay(1500); // Maybe do not need this or can be shorter?

  // Initialize time of flight sensor
  if (!ToF.begin()) {
      Serial.println(F("Failed to boot VL53L0X"));
      while(1);
  }
}

void loop(){

  VL53L0X_RangingMeasurementData_t val = read_ToF(ToF);
  bool ToF_flag;

  if (val.RangeStatus != 4) {  // phase failures have incorrect data
    ToF_flag = true;
  } 
  else {
    ToF_flag = false;
  }

  // Send packet
  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  if (ToF_flag){
    LoRa.print("Distance = ");
    LoRa.print(val.RangeMilliMeter);
    LoRa.print(" mm ");
  }
  else{
    LoRa.print("Distance = ??");
  }
  LoRa.endPacket();

  delay(100);

}
