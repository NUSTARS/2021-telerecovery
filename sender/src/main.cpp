/*
  Adapted from
  ...\heltec_wifi_lora_32_V2\Heltec ESP32 Dev-Boards\examples\LoRa\OLED_LoRa_Sender\OLED_LoRa_Sender.ino
*/
#include <Arduino.h>
#include "heltec.h"
#include "ToF.h"
#include "IMU.h"

#define BAND 915E6  // You can set band here directly,e.g. 868E6,915E6

#define DT  0.02          // Loop time


Adafruit_VL53L0X ToF = Adafruit_VL53L0X();

byte buff[6];
int accRaw[3];
int gyrRaw[3];
float gyroXangle = 0.0;
float gyroYangle = 0.0;
float gyroZangle = 0.0;
float AccYangle = 0.0;
float AccXangle = 0.0;

void setup(){

  // Start ESP32 Board
  Heltec.begin(
    false /*DisplayEnable Enable*/,
    true /*Heltec.Heltec.Heltec.LoRa Disable*/, 
    true /*Serial Enable*/, 
    true /*PABOOST Enable*/, 
    BAND /*long BAND*/);
 
  delay(1500); // Maybe do not need this or can be shorter?

  // Initialize time of flight sensor
  if (!ToF.begin()) {
      Serial.println(F("Failed to boot VL53L0X"));
      while(1);
  }

  // Initialize the IMU
  detectIMU();
  enableIMU();

  Serial.begin(115200); // init serial for testing
  delay(500);

}

void loop(){

  //Read the measurements from  sensors
  readACC(buff);
  accRaw[0] = (int)(buff[0] | (buff[1] << 8));   
  accRaw[1] = (int)(buff[2] | (buff[3] << 8));
  accRaw[2] = (int)(buff[4] | (buff[5] << 8));

  //Convert Accelerometer values to degrees
  AccXangle = (float) (atan2(accRaw[1],accRaw[2])+M_PI)*RAD_TO_DEG;
  AccYangle = (float) (atan2(accRaw[2],accRaw[0])+M_PI)*RAD_TO_DEG;

  //If IMU is up the correct way, use these lines
        AccXangle -= (float)180.0;
  if (AccYangle > 90)
          AccYangle -= (float)270;
  else
    AccYangle += (float)90;


  VL53L0X_RangingMeasurementData_t val = read_ToF(ToF);
  bool ToF_flag;

  if (val.RangeStatus != 4) {  // phase failures have incorrect data
    ToF_flag = true;
  } 
  else {
    ToF_flag = false;
  }

  Serial.print("AccX=");
  Serial.print(AccXangle);
  Serial.print("\tAccY=");
  Serial.print(AccYangle);
  Serial.print("\tDistance=");


  if (ToF_flag){
    Serial.println(val.RangeMilliMeter);
  }
  else{
    Serial.println("?");
  }


  // // Send packet
  // LoRa.beginPacket();
  // LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  // if (ToF_flag){
  //   LoRa.print("Distance = ");
  //   LoRa.print(val.RangeMilliMeter);
  //   LoRa.print(" mm ");
  // }
  // else{
  //   LoRa.print("Distance = ??");
  // }
  // LoRa.endPacket();

  // delay(100);

}
