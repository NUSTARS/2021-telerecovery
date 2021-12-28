#include <Adafruit_Sensor_Calibration.h>
#include "transmitter.h"
Adafruit_Sensor *accelerometer, *gyroscope, *magnetometer; // IMU

// uncomment one combo 9-DoF!
#include "LSM6DS_LIS3MDL.h"  // see the the LSM6DS_LIS3MDL file in this project to change board to LSM6DS33, LSM6DS3U, LSM6DSOX, etc
//#include "LSM9DS.h"           // LSM9DS1 or LSM9DS0
//#include "NXP_FXOS_FXAS.h"  // NXP 9-DoF breakout

// select either EEPROM or SPI FLASH storage:
#ifdef ADAFRUIT_SENSOR_CALIBRATION_USE_EEPROM
  Adafruit_Sensor_Calibration_EEPROM cal;
#else
  Adafruit_Sensor_Calibration_SDFat cal;
#endif


// Declare sensor objects
Adafruit_VL53L0X ToF = Adafruit_VL53L0X(); // time of flight
Adafruit_BMP3XX bmp; // pressure                  

// variables to store IMU data
sensors_event_t mag_event, gyro_event, accel_event;

unsigned long startTime; // time for loop
int loopcount = 0;

void setup(void) {

  // Start ESP32 Board
  Heltec.begin(
      false /*DisplayEnable Enable*/,
      true /*LoRa enable*/,
      true /*Serial Enable*/,
      true /*PABOOST Enable*/,
      BAND /*long BAND*/);
  delay(500); 

  Serial.begin(115200); // init serial for testings
  while (!Serial) delay(10);  

  // Initialize time of flight sensor
  if (!ToF.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1) {;}
  }

  // Initialize BMP388 pressure sensor
  if (!bmp.begin_I2C()) {
    Serial.println("Failed to boot BMP388");
    while (1);
  }

  // Set up oversampling and filter initialization on the BMP388
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);


  // Initialize calibration helper code
  if (!cal.begin()) {
    Serial.println("Failed to initialize calibration helper");
    while (1) yield();
  }

  // Load existing calibration from EEPROM if its there, or use default
  if (! cal.loadCalibration()) {
    Serial.println("No calibration loaded/found... will start with defaults");
  } else {
    Serial.println("Loaded existing calibration");
  }

  // Initialize IMU sensors
  if (!init_sensors()) {
    Serial.println("Failed to find sensors");
    while (1) delay(10);
  }

  // Print out sensor information
  // accelerometer->printSensorDetails();
  // gyroscope->printSensorDetails();
  // magnetometer->printSensorDetails();

  setup_sensors();
  
  Wire.setClock(400000); // 400KHz
}


void loop() {

  startTime = millis();

  // Read pressure sensor reading 
  if (!bmp.performReading()) {
    return;
  }

  // Read time of flight sensor data
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


  // Read IMU data
  magnetometer->getEvent(&mag_event);
  gyroscope->getEvent(&gyro_event);
  accelerometer->getEvent(&accel_event);
  
  // Pack up the data into an array
  char buffer[512];
  char data_format[] = "%hu,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f";

  sprintf(buffer,data_format,
    tof_data,
    bmp.temperature,
    bmp.pressure,
    bmp.readAltitude(SEALEVELPRESSURE_HPA),
    accel_event.acceleration.x,
    accel_event.acceleration.y,
    accel_event.acceleration.z,
    gyro_event.gyro.x,
    gyro_event.gyro.y,
    gyro_event.gyro.z,
    mag_event.magnetic.x,
    mag_event.magnetic.y,
    mag_event.magnetic.z
  );

  // Serial.print(buffer);

  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(buffer);
  LoRa.endPacket();

  //Each loop should be at least 20ms.
  while(millis() - startTime < (DT*1000)) {
    delay(1);
  }
 
} // END LOOP
