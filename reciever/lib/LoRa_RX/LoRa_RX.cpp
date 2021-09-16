#include "LoRa_RX.h"
#include "OLED.h"

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  
  for (int i = 0; i < packetSize; i++){
    packet += (char) LoRa.read(); 
  }

  rssi = "RSSI = " + String(LoRa.packetRssi(), DEC) ;

  OLED_print(0,0,MAX_SCREEN_HEIGHT,rssi);
  // print_LoRa_data(0,26,128,"Packet recieved",rssi);

  Serial.println(packet); // Print packet over UART -> to JS app
}

