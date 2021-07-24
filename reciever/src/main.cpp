/*
  Adapted from ...\examples\LoRa\OLED_LoRa_Reciever.ino

  TODO: 
    - Reintroduce RSSI reading because it is probably useful
        RSSI minimum = -120 dBm.
        If RSSI=-30dBm: signal is strong.
        If RSSI=-120dBm: signal is weak.

    - Decide how best to organize (where to put cbk function)
    
    - Do I keep those same global variables?

*/
#include <Arduino.h>
#include "OLED.h"
#include "LoRa_RX.h"

String rssi = "RSSI --";
String packSize = "--";
String packet;

void setup() { 

  Heltec.begin(
    true /*Display Enabled*/,
    true /*Heltec.Heltec.Heltec.LoRa Disable*/, 
    true /*Serial Enable*/, 
    true /*PABOOST Enable*/, 
    BAND /*long BAND*/);

  OLED_init();

  LoRa.receive();
}

void loop() {
  int packetSize = LoRa.parsePacket();
  
  if (packetSize){
    cbk(packetSize);
  }

  delay(10);
}