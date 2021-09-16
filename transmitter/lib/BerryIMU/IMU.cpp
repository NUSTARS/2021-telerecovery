#include "IMU.h"

void writeTo(int device, unsigned char address, unsigned char val)
{
  Wire.beginTransmission(device); //start transmission to device
  Wire.write(address);            // send register address
  Wire.write(val);                // send value to write
  Wire.endTransmission();         //end transmission
}

void readFrom(int device, unsigned char address, int num, unsigned char buff[])
{
  Wire.beginTransmission(device); //start transmission to device
  Wire.write(address);            //sends address to read from
  Wire.endTransmission();         //end transmission

  Wire.beginTransmission(device); //start transmission to device (initiate again)
  Wire.requestFrom(device, num);  // request 6  unsigned chars from device

  int i = 0;
  while (Wire.available()) //device may send less than requested (abnormal)
  {
    buff[i] = Wire.read(); // receive a  unsigned char
    i++;
  }
  Wire.endTransmission(); //end transmission
}

void detectIMU()
{

  Wire.begin();
  unsigned char LSM6DSL_WHO_AM_I_response; // Accelerometer and gyroscope
  unsigned char LIS3MDL_WHO_AM_I_response; // Magnetometer
  unsigned char BMP388_WHO_AM_I_response;  // Pressure Sensor

  unsigned char WHOresponse[3];

  //Detect if LSM6DSL (acc and gyro), LIS3MDL (magnetometer), and BMP388 (pressure) sensors are connected
  readFrom(LSM6DSL_ADDRESS, LSM6DSL_WHO_AM_I, 1, WHOresponse);
  LSM6DSL_WHO_AM_I_response = WHOresponse[0];

  readFrom(LIS3MDL_ADDRESS, LIS3MDL_WHO_AM_I, 1, WHOresponse);
  LIS3MDL_WHO_AM_I_response = WHOresponse[0];

  readFrom(I2C_ADD_BMP388, BMP388_REG_ADD_WIA, 1, WHOresponse);
  BMP388_WHO_AM_I_response = WHOresponse[0];

  if (LSM6DSL_WHO_AM_I_response == 0x6A && LIS3MDL_WHO_AM_I_response == 0x3D && BMP388_WHO_AM_I_response == BMP388_REG_VAL_WIA)
  {
    Serial.println("\n\n   BerryIMUv3(LSM6DSL, LIS3MLD, & BMP388) found \n\n");
  }

  delay(1000);
}

void enableIMU()
{
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
  // readFrom(LSM6DSL_ADDRESS, LIS3MDL_CTRL_REG3, 1, temp);

  // initialize pressure/temperature sensor
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
  
}

void readACC(unsigned char buff[])
{
  readFrom(LSM6DSL_ADDRESS, LSM6DSL_OUT_X_L_XL, 6, buff);
}

void readMAG(unsigned char buff[])
{
  readFrom(LIS3MDL_ADDRESS, 0x80 | LIS3MDL_OUT_X_L, 6, buff);
}

void readGYR(unsigned char buff[])
{
  readFrom(LSM6DSL_ADDRESS, LSM6DSL_OUT_X_L_G, 6, buff);
}
