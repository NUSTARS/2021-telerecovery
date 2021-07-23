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

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6

String rssi = "RSSI --";
String packSize = "--";
String packet ;

// void LoRaData(){
//   Heltec.display->clear();
//   Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
//   Heltec.display->setFont(ArialMT_Plain_10);
//   // Heltec.display->drawString(0 , 15 , "Received "+ packSize + " bytes");
//   Heltec.display->drawStringMaxWidth(0 , 26 , 128, packet);
//   // Heltec.display->drawString(0, 0, rssi);  
//   Heltec.display->display();
// }

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  
  for (int i = 0; i < packetSize; i++){
    packet += (char) LoRa.read(); 
  }

  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  printText(0,26,128,packet);
}

void setup() { 

  Heltec.begin(
    true /*Display Enabled*/,
    true /*Heltec.Heltec.Heltec.LoRa Disable*/, 
    true /*Serial Enable*/, 
    true /*PABOOST Enable*/, 
    BAND /*long BAND*/);

  OLED_init();

  // LoRa.onReceive(cbk);
  LoRa.receive();
}

void loop() {
  int packetSize = LoRa.parsePacket();
  
  if (packetSize){
    cbk(packetSize);
  }

  delay(10);
}