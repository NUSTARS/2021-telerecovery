#include "OLED.h"

void OLED_init(void){
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  OLED_logo();
  delay(1500);
  Heltec.display->clear();

  Heltec.display->drawString(0, 0, "SkyNet System Terminal");
  Heltec.display->drawString(0, 10, "Initialization Success!");
  Heltec.display->drawString(0, 20, "Waiting for data from core...");
  Heltec.display->display();
}

void OLED_logo(void){
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

void OLED_print(int16_t x, int16_t y, uint16_t maxLineWidth, String msg) {
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawStringMaxWidth(x, y, maxLineWidth, msg); 
  Heltec.display->display();
}

void OLED_clear(void) {
  Heltec.display->clear();
}