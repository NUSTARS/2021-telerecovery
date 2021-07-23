
#ifndef OLED_H_
#define OLED_H_

#include "heltec.h"
#include "images.h"

void OLED_init(void);
void logo(void);
void printText(int16_t x, int16_t y, uint16_t maxLineWidth, String text);

#endif