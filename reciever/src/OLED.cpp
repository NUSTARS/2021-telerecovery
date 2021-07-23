#include "OLED.h"

void OLED_init(void){
    Heltec.display->init();
    Heltec.display->flipScreenVertically();  
    Heltec.display->setFont(ArialMT_Plain_10);
    logo();
    delay(1500);
    Heltec.display->clear();

    Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
    Heltec.display->drawString(0, 10, "Waiting for data...");
    Heltec.display->display();
    delay(500);
}

void logo(void){
  // TODO: Would be cool to change to NUSTARS logo
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}


void printText(int16_t x, int16_t y, uint16_t maxLineWidth, String text){
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawStringMaxWidth(x, y, maxLineWidth, text); 
  Heltec.display->display();
}
