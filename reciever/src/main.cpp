/*
  Adapted from .pio\libdeps\heltec_wifi_lora_32_V2\\examples\LoRa\OLED_LoRa_Reciever.ino
*/

// #include "reciever.h"
#include <Arduino.h>
#include "OLED.h"
#include "LoRa_RX.h"
#define DT  0.02

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

  OLED_init(); // Initialize the OLED screen
 
  LoRa.setSignalBandwidth(250E3); // must match transmitter
 
  LoRa.receive(); // Initialize LoRa for recieving
}

int count = 0;
unsigned long startTime;

void loop() {

  startTime = millis();

  char msg[50];

  int packetSize = LoRa.parsePacket();
  
  if (packetSize){
    OLED_clear();

    cbk(packetSize); // recieve and print recieved packet

    count ++;
    sprintf(msg,"Packet received: %d",count);
    OLED_print(0,26,MAX_SCREEN_WIDTH,msg);
  }

  // Each loop should be at least 20ms.
  while(millis() - startTime < (DT*1000)) {
    // wait
  }
  
}
