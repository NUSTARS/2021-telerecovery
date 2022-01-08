/*
  Adapted from .pio\libdeps\heltec_wifi_lora_32_V2\\examples\LoRa\OLED_LoRa_Reciever.ino
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

int count = 0;

void loop() {
  char msg[50];

  int packetSize = LoRa.parsePacket();
  
  if (packetSize){
    OLED_clear();

    cbk(packetSize);

    count++;
    sprintf(msg,"New Packet #%d",count);
    OLED_print(0,26,MAX_SCREEN_WIDTH,msg);
  }

  delay(20);
}