
#ifndef OLED_H_
#define OLED_H_

#include "heltec.h"
#include "boot_logo.h"

void OLED_init(void);
void logo(void);
void print_LoRa_data(int16_t x, int16_t y, uint16_t maxLineWidth, String packet, String rssi);

#endif