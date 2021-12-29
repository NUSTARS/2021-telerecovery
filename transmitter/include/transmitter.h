// Project header file which includes important headers and defines some constants
#include <Arduino.h>
#include <Adafruit_Sensor_Calibration.h>
#include "heltec.h"
#include "ToF.h"
#include "IMU.h"
#include "Adafruit_BMP3XX.h"

#define BAND 915E6  // You can set band here directly,e.g. 868E6,915E6
#define SEALEVELPRESSURE_HPA (1013.25)

#define DT  0.02          // Loop time
#define AA  0.97         // complementary filter constant
#define G_GAIN 0.070    // [deg/s/LSB]  

#ifndef M_PI
  #define M_PI 3.141592653589793
#endif

// Variables to store data
unsigned char buff[6];
int accRaw[3];
int gyrRaw[3];

float gyroXangle = 0.0;
float gyroYangle = 0.0;
float gyroZangle = 0.0;

float rate_gyr_y = 0.0; // [deg/s]
float rate_gyr_x = 0.0; // [deg/s]
float rate_gyr_z = 0.0; // [deg/s]

float AccYangle = 0.0;
float AccXangle = 0.0;
// float AccZangle = 0.0;

float CFangleX = 0.0;
float CFangleY = 0.0;