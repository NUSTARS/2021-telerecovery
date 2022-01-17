#include "LoRa_RX.h"
#include "OLED.h"

uint8_t ToF;
uint8_t temp_temperature[4];
uint8_t temp_pressure[4];
uint8_t temp_altitude[4];
uint8_t temp_accelx[4];
uint8_t temp_accely[4];
uint8_t temp_accelz[4];
uint8_t temp_gyrox[4];
uint8_t temp_gyroy[4];
uint8_t temp_gyroz[4];
uint8_t temp_magx[4];
uint8_t temp_magy[4];
uint8_t temp_magz[4];

float temperature;
float pressure;
float altitude;
float accelx;
float accely;
float accelz;
float gyrox;
float gyroy;
float gyroz;
float magx;
float magy;
float magz;

void cbk(int packetSize) {
  packSize = String(packetSize,DEC);
  ToF = LoRa.read();
  LoRa.readBytes(temp_temperature, 4);
  LoRa.readBytes(temp_pressure, 4);
  LoRa.readBytes(temp_altitude, 4);
  LoRa.readBytes(temp_accelx, 4);
  LoRa.readBytes(temp_accely, 4);
  LoRa.readBytes(temp_accelz, 4);
  LoRa.readBytes(temp_gyrox, 4);
  LoRa.readBytes(temp_gyroy, 4);
  LoRa.readBytes(temp_gyroz, 4);
  LoRa.readBytes(temp_magx, 4);
  LoRa.readBytes(temp_magy, 4);
  LoRa.readBytes(temp_magz, 4);

  // Oled show RSSI
  rssi = "RSSI = " + String(LoRa.packetRssi(), DEC) ;
  OLED_print(0,0,MAX_SCREEN_HEIGHT,rssi);

// // Pack up the data into an array
char buffer[150];
char data_format[] = "%u,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%i";

temperature = *((float *)temp_temperature);
pressure = *((float *)temp_pressure);
altitude = *((float *)temp_altitude);
accelx = *((float *)temp_accelx);
accely = *((float *)temp_accely);
accelz = *((float *)temp_accelz);
gyrox = *((float *)temp_gyrox);
gyroy = *((float *)temp_gyroy);
gyroz = *((float *)temp_gyroz);
magx = *((float *)temp_magx);
magy = *((float *)temp_magy);
magz = *((float *)temp_magz);

  sprintf(buffer,data_format,
    ToF,
    temperature,
    pressure,
    altitude,
    accelx,
    accely,
    accelz,
    gyrox,
    gyroy,
    gyroz,
    magx,
    magy,
    magz,
    LoRa.packetRssi() // Append package RSSI to printout
  );

  Serial.println(buffer); // Print packet over UART -> to JS app
}

