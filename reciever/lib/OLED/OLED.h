
#ifndef OLED_H_
#define OLED_H_

#include "heltec.h"
#include "boot_logo.h"

#define MAX_SCREEN_HEIGHT 53
#define MAX_SCREEN_WIDTH 128

void OLED_init(void);
void logo(void);
void print_LoRa_data(int16_t x, int16_t y, uint16_t maxLineWidth, String packet, String rssi);
void OLED_print(int16_t x, int16_t y, uint16_t maxLineWidth, String msg);
void OLED_clear(void);

#endif