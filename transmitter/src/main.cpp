#include "transmitter.h"

Adafruit_VL53L0X ToF = Adafruit_VL53L0X(); // Declare time of flight sensor object
Adafruit_BMP3XX bmp;                       // Declare pressure sensor object


unsigned char buff[6];
int accRaw[3];
int gyrRaw[3];

float gyroXangle = 0.0;
float gyroYangle = 0.0;
float gyroZangle = 0.0;

float rate_gyr_y = 0.0;   // [deg/s]
float rate_gyr_x = 0.0;    // [deg/s]
float rate_gyr_z = 0.0;     // [deg/s]

float AccYangle = 0.0;
float AccXangle = 0.0;

float CFangleX = 0.0;
float CFangleY = 0.0;

unsigned long startTime;
void setup()
{

  // Start ESP32 Board
  Heltec.begin(
      false /*DisplayEnable Enable*/,
      true /*Heltec.Heltec.Heltec.LoRa Disable*/,
      true /*Serial Enable*/,
      true /*PABOOST Enable*/,
      BAND /*long BAND*/);

  delay(500); // Maybe do not need this or can be shorter?

  // Initialize time of flight sensor
  if (!ToF.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }


  Serial.begin(115200); // init serial for testing
  delay(500);

  // Initialize the IMU
  detectIMU();
  enableIMU();

  if (!bmp.begin_I2C()) {
    Serial.println("Failed to boot BMP388");
    while (1);
  }

  // Set up oversampling and filter initialization on the BMP388
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
}

void loop() {
  startTime = millis();

  if (!bmp.performReading()) {
    return;
  }

  // Read raw acceleration data
  readACC(buff);
  accRaw[0] = (int)(buff[0] | (buff[1] << 8));
  accRaw[1] = (int)(buff[2] | (buff[3] << 8));
  accRaw[2] = (int)(buff[4] | (buff[5] << 8));

  // Read raw gyroscope data
  readGYR(buff);
  gyrRaw[0] = (int)(buff[0] | (buff[1] << 8));   
  gyrRaw[1] = (int)(buff[2] | (buff[3] << 8));
  gyrRaw[2] = (int)(buff[4] | (buff[5] << 8));

  //Convert Accelerometer values to degrees
  AccXangle = (float)(atan2(accRaw[1], accRaw[2]) + M_PI) * RAD_TO_DEG;
  AccYangle = (float)(atan2(accRaw[2], accRaw[0]) + M_PI) * RAD_TO_DEG;

  //If IMU is up the correct way, use these lines
  AccXangle -= (float)180.0;
  if (AccYangle > 90)
  {
    AccYangle -= (float)270;
  }
  else
  {
    AccYangle += (float)90;
  }

  //Convert Gyro raw to degrees per second
  rate_gyr_x = (float) gyrRaw[0] * G_GAIN;
  rate_gyr_y = (float) gyrRaw[1]  * G_GAIN;
  rate_gyr_z = (float) gyrRaw[2]  * G_GAIN;

  //Calculate the angles from the gyro
  gyroXangle+=rate_gyr_x*DT;
  gyroYangle+=rate_gyr_y*DT;
  gyroZangle+=rate_gyr_z*DT;

  //Complementary filter used to combine the accelerometer and gyro values.
  CFangleX=AA*(CFangleX+rate_gyr_x*DT) +(1 - AA) * AccXangle;
  CFangleY=AA*(CFangleY+rate_gyr_y*DT) +(1 - AA) * AccYangle;


  VL53L0X_RangingMeasurementData_t val = read_ToF(ToF);

  uint16_t tof_data=0;
  if (val.RangeStatus != 4)
  {
    tof_data = val.RangeMilliMeter;
  }
  else
  {
    tof_data = 0;
  }

  // Pack up the data into an array
  char buffer[128];
  char data_format[] = "%f,%f,%hu,%f,%f,%f,%f,%f,%f,%f,%f\n";
  sprintf(buffer, data_format, 
  AccXangle,
  AccYangle,
  tof_data,
  bmp.temperature,
  bmp.pressure,
  bmp.readAltitude(SEALEVELPRESSURE_HPA),
  gyroXangle,
  gyroYangle,
  gyroZangle,
  CFangleX,
  CFangleY);

  // Serial.print(buffer);
  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(buffer);
  LoRa.endPacket();

  //Each loop should be at least 20ms.
  while(millis() - startTime < (DT*1000)) {
    delay(1);
  }
}
