#include <Arduino.h>
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


