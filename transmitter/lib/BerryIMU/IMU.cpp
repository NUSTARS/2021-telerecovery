#include <Arduino.h>
#include <Wire.h>
#include "LSM6DSL.h"
#include "LIS3MDL.h"
#include "BMP388.h"


int BerryIMUversion = 99;

void writeTo(int device, unsigned char address,  unsigned char val) {
   Wire.beginTransmission(device); //start transmission to device 
   Wire.write(address);        // send register address
   Wire.write(val);        // send value to write
   Wire.endTransmission(); //end transmission
}


void readFrom(int device, unsigned char address, int num,  unsigned char buff[]) {
  Wire.beginTransmission(device); //start transmission to device 
  Wire.write(address);        //sends address to read from
  Wire.endTransmission(); //end transmission
  
  Wire.beginTransmission(device); //start transmission to device (initiate again)
  Wire.requestFrom(device, num);    // request 6  unsigned chars from device
  
  int i = 0;
  while(Wire.available())    //device may send less than requested (abnormal)
  { 
    buff[i] = Wire.read(); // receive a  unsigned char
    i++;
  }
  Wire.endTransmission(); //end transmission
}



void detectIMU(){
  //Detect which version of BerryIMU is connected using the 'who am i' register
  //BerryIMUv1 uses the LSM9DS0
  //BerryIMUv2 uses the LSM9DS1
  //BerryIMUv3 uses the LSM6DSL and LIS3MDL
  
  Wire.begin(); 
  //  unsigned char LSM9DS0_WHO_AM_I_G_response;
  //  unsigned char LSM9DS0_WHO_AM_I_XM_response;
  //  unsigned char LSM9DS1_WHO_M_response;
  //  unsigned char LSM9DS1_WHO_XG_response;
   unsigned char LSM6DSL_WHO_AM_I_response;
   unsigned char LIS3MDL_WHO_AM_I_response;

  
   unsigned char WHOresponse[2];
  
  //Detect if BerryIMUv3 (Which uses the LSM6DSL and LIS3MDL) is connected
  readFrom(LSM6DSL_ADDRESS, LSM6DSL_WHO_AM_I,1,WHOresponse);
  LSM6DSL_WHO_AM_I_response = WHOresponse[0];
  
  readFrom(LIS3MDL_ADDRESS, LIS3MDL_WHO_AM_I,1,WHOresponse);
  LIS3MDL_WHO_AM_I_response = WHOresponse[0];

  if (LSM6DSL_WHO_AM_I_response == 0x6A && LIS3MDL_WHO_AM_I_response == 0x3D){
    Serial.println("\n\n   BerryIMUv3(LSM6DSL & LIS3MLD) found \n\n");
    BerryIMUversion = 3;
    
  }
  
  delay(1000);
  
}


void enableIMU(){

if(BerryIMUversion == 3){//For BerryIMUv3

  //initialise the accelerometer
  writeTo(LSM6DSL_ADDRESS, LSM6DSL_CTRL1_XL, 0b10011111); // ODR 3.33 kHz, +/- 8g , BW = 400hz
  writeTo(LSM6DSL_ADDRESS, LSM6DSL_CTRL8_XL, 0b11001000); // Low pass filter enabled, BW9, composite filter
  writeTo(LSM6DSL_ADDRESS, LSM6DSL_CTRL3_C, 0b01000100);  // Enable Block Data update, increment during multi  unsigned char read

  //initialise the gyroscope
  writeTo(LSM6DSL_ADDRESS, LSM6DSL_CTRL2_G, 0b10011100); // ODR 3.3 kHz, 2000 dps

  //initialise the magnetometer
  writeTo(LIS3MDL_ADDRESS, LIS3MDL_CTRL_REG1, 0b11011100); // Temp sesnor enabled, High performance, ODR 80 Hz, FAST ODR disabled and Selft test disabled.
  writeTo(LIS3MDL_ADDRESS, LIS3MDL_CTRL_REG2, 0b00100000); // +/- 8 gauss
  writeTo(LIS3MDL_ADDRESS, LIS3MDL_CTRL_REG3, 0b00000000); // Continuous-conversion mode
  unsigned char temp[11];
    readFrom(LSM6DSL_ADDRESS,LIS3MDL_CTRL_REG3,1, temp);
    // Serial.print(temp);
  }
}

void readACC(unsigned char buff[]){
  readFrom(LSM6DSL_ADDRESS, LSM6DSL_OUT_X_L_XL, 6, buff);  
} 
 
void readMAG(unsigned char buff[]){
  readFrom(LIS3MDL_ADDRESS, 0x80 | LIS3MDL_OUT_X_L, 6, buff);  
}

void readGYR(unsigned char buff[]){
  readFrom(LSM6DSL_ADDRESS, LSM6DSL_OUT_X_L_G, 6, buff);
}

