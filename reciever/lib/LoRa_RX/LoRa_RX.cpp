#include "LoRa_RX.h"
#include "OLED.h"

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  
  for (int i = 0; i < packetSize; i++){
    packet += (char) LoRa.read(); 
  }

  // Append package RSSI to printout
  packet += ("," + String(LoRa.packetRssi()));
  rssi = "RSSI = " + String(LoRa.packetRssi(), DEC) ;

  OLED_print(0,0,MAX_SCREEN_HEIGHT,rssi);

  Serial.println(packet); // Print packet over UART -> to JS app
}

