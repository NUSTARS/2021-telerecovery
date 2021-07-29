#include "LoRa_RX.h"
#include "OLED.h"

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  
  for (int i = 0; i < packetSize; i++){
    packet += (char) LoRa.read(); 
  }

  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  
  print_LoRa_data(0,26,128,packet,rssi);

  Serial.println(packet);
}

