/*
  Adapted from ...\examples\LoRa\OLED_LoRa_Reciever.ino
*/
#include <Arduino.h>
#include "OLED.h"
#include "LoRa_RX.h"

String rssi = "RSSI --";
String packSize = "--";
String packet;

void setup() { 
  Serial.begin(115200);
  
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