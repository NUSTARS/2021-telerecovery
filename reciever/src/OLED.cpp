#include "OLED.h"

void OLED_init(void){
    Heltec.display->init();
    Heltec.display->flipScreenVertically();  
    Heltec.display->setFont(ArialMT_Plain_10);
    logo();
    delay(1500);
    Heltec.display->clear();

    Heltec.display->drawString(0, 0, "Initialization success!");
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


void print_LoRa_data(int16_t x, int16_t y, uint16_t maxLineWidth, String packet, String rssi){
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 0, rssi);  
  Heltec.display->drawStringMaxWidth(x, y, maxLineWidth, packet); 
  Heltec.display->display();
}
