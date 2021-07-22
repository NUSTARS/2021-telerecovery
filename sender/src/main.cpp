/*

  Adapted from OLED_LoRa_Sender

*/
#include <Arduino.h>
#include "heltec.h"
#include "images.h"
#include "ToF.h"

#define BAND 915E6  // You can set band here directly,e.g. 868E6,915E6

Adafruit_VL53L0X ToF = Adafruit_VL53L0X();

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
  bool ToF_flag;

  if (val.RangeStatus != 4) {  // phase failures have incorrect data
    ToF_flag = true;
      // Serial.print("Distance (mm): "); 
      // Serial.println(val.RangeMilliMeter);
  } 
  else {
    ToF_flag = false;
    // Serial.println(" Out of range ");
  }

  // Send packet
  // TODO: make into a separate function
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
