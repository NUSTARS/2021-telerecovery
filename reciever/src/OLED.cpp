#include "OLED.h"

void OLED_init(void){
    Heltec.display->init();
    Heltec.display->flipScreenVertically();  
    Heltec.display->setFont(ArialMT_Plain_10);
    logo();
    delay(1500);
    Heltec.display->clear();
}

void logo(void){
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}


