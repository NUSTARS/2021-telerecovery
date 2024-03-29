#include "transmitter.h"  

// Declare sensor objects
Adafruit_VL53L0X ToF = Adafruit_VL53L0X(); // time of flight
Adafruit_BMP3XX bmp; // pressure                  

// variables to store IMU data
Adafruit_Sensor *accelerometer, *gyroscope, *magnetometer; // IMU
// see the the LSM6DS_LIS3MDL file in this project to change board to LSM6DS33, LSM6DS3U, LSM6DSOX, etc
#include "LSM6DS_LIS3MDL.h"
sensors_event_t mag_event, gyro_event, accel_event;

unsigned long startTime; // time for loop
int loopcount = 0;

/*--------------------------------------------------*/
void setup(void) {

  // Start Heltec LoRa ESP32 Board
  Heltec.begin(
      false /*DisplayEnable Enable*/,
      true /*LoRa enable*/,
      true /*Serial Enable*/,
      true /*PABOOST Enable*/,
      BAND /*long BAND*/);
  delay(500); // wait for board to be initialized

  Serial.begin(115200); // init serial for testings
  while (!Serial) delay(10);  

  pinMode(21, OUTPUT);
  digitalWrite(21, LOW); // Enable Vext

  // Initialize time of flight sensor
  if (!ToF.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1) {;}
  } else {
    Serial.println(F("Booted VL53L0X"));
  }

  // Initialize BMP388 pressure sensor
  if (!bmp.begin_I2C()) {
    Serial.println(F("Failed to boot BMP388"));
    while (1);
  } else {
    Serial.println(F("Booted BPM388"));
  }

  // Set up oversampling and filter initialization on the BMP388
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  // Uncomment Serial.print statements for debugging 

  // Initialize IMU sensors
  if (!init_sensors()) {
    Serial.println(F("Failed to find IMU sensors"));
    while (1) delay(10);
  } else {
    Serial.println(F("Booted IMU sensors"));
  }

  // Print out sensor information
  accelerometer->printSensorDetails();
  gyroscope->printSensorDetails();
  magnetometer->printSensorDetails();

  setup_sensors();
  
  Wire.setClock(400000); // 400KHz
}

void loop() {

  startTime = millis();

  // Read pressure sensor (float)
  if (!bmp.performReading()) {
    return;
  }

  // Read time of flight sensor (unsigned short)
  VL53L0X_RangingMeasurementData_t val = read_ToF(ToF);
  uint16_t tof_data=0;
  if (val.RangeStatus != 4) {
    tof_data = val.RangeMilliMeter;
  } else {
    // if data is bad (probably nothing in range) set equal to zero
    tof_data = 0;
  }

  // Read IMU data (all floats)
  magnetometer->getEvent(&mag_event);
  gyroscope->getEvent(&gyro_event);
  accelerometer->getEvent(&accel_event);
  
  // Pack up the data into an array
  char buffer[150];
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

  float temptemperature = bmp.temperature;
  float temppressure = bmp.pressure;
  float tempaltitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  
  LoRa.beginPacket();
  LoRa.setSignalBandwidth(500E3);
  LoRa.setTxPower(20,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.write((uint8_t)tof_data);
  LoRa.write((uint8_t *) &temptemperature, 4);
  LoRa.write((uint8_t *) &temppressure, 4);
  LoRa.write((uint8_t *) &tempaltitude, 4);
  LoRa.write((uint8_t *) &accel_event.acceleration.x, 4);
  LoRa.write((uint8_t *) &accel_event.acceleration.y, 4);
  LoRa.write((uint8_t *) &accel_event.acceleration.z, 4);
  LoRa.write((uint8_t *) &gyro_event.gyro.x, 4);
  LoRa.write((uint8_t *) &gyro_event.gyro.y, 4);
  LoRa.write((uint8_t *) &gyro_event.gyro.z, 4);
  LoRa.write((uint8_t *) &mag_event.magnetic.x, 4);
  LoRa.write((uint8_t *) &mag_event.magnetic.y, 4);
  LoRa.write((uint8_t *) &mag_event.magnetic.z, 4);
  LoRa.endPacket();

  // Each loop should be at least 20ms.
  while(millis() - startTime < (DT*1000)) {
    // wait
  }
 
} // END LOOP
