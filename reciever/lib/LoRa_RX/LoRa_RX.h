#ifndef LORARX_H_
#define LORARX_H_

#include <Arduino.h>
#include "heltec.h"

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6

/* Global variables*/
extern String rssi;
extern String packSize;
extern String packet;

/* onReceive LoRa Packet */
int onReceive(int packetSize); 

#endif