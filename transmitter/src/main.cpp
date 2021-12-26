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

// function prototypes
// uint16_t crc16_update(uint16_t crc, uint8_t a);
// void receiveCalibration();



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
  accelerometer->printSensorDetails();
  gyroscope->printSensorDetails();
  magnetometer->printSensorDetails();

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
  char data_format[] = "ToF=%hu,T=%f,P=%f,Alt=%f,a_x%f,a_y%f,a_z%f,g_x%f,g_y%f,g_z=%f,m_x=%f,m_y=%f,m_z=%f\n";

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

  loopcount++;
  // receiveCalibration();

  // // occasionally print calibration
  // if (loopcount == 50 || loopcount > 100) {
  //   Serial.print("Cal1:");
  //   for (int i=0; i<3; i++) {
  //     Serial.print(cal.accel_zerog[i], 3); 
  //     Serial.print(",");
  //   }
  //   for (int i=0; i<3; i++) {
  //     Serial.print(cal.gyro_zerorate[i], 3);
  //     Serial.print(",");
  //   }  
  //   for (int i=0; i<3; i++) {
  //     Serial.print(cal.mag_hardiron[i], 3); 
  //     Serial.print(",");
  //   }  
  //   Serial.println(cal.mag_field, 3);
  //   loopcount++;
  // }
  // if (loopcount >= 100) {
  //   Serial.print("Cal2:");
  //   for (int i=0; i<9; i++) {
  //     Serial.print(cal.mag_softiron[i], 4); 
  //     if (i < 8) Serial.print(',');
  //   }
  //   Serial.println();
  //   loopcount = 0;
  // }

  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(buffer);
  LoRa.endPacket();

  //Each loop should be at least 20ms.
  while(millis() - startTime < (DT*1000)) {
    delay(1);
  }
 
  // delay(10); 

} // END LOOP

/********************************************************/

// byte caldata[68]; // buffer to receive magnetic calibration data
// byte calcount=0;

// void receiveCalibration() {
//   uint16_t crc;
//   byte b, i;

//   while (Serial.available()) {
//     b = Serial.read();
//     if (calcount == 0 && b != 117) {
//       // first byte must be 117
//       return;
//     }
//     if (calcount == 1 && b != 84) {
//       // second byte must be 84
//       calcount = 0;
//       return;
//     }
//     // store this byte
//     caldata[calcount++] = b;
//     if (calcount < 68) {
//       // full calibration message is 68 bytes
//       return;
//     }
//     // verify the crc16 check
//     crc = 0xFFFF;
//     for (i=0; i < 68; i++) {
//       crc = crc16_update(crc, caldata[i]);
//     }
//     if (crc == 0) {
//       // data looks good, use it
//       float offsets[16];
//       memcpy(offsets, caldata+2, 16*4);
//       cal.accel_zerog[0] = offsets[0];
//       cal.accel_zerog[1] = offsets[1];
//       cal.accel_zerog[2] = offsets[2];
      
//       cal.gyro_zerorate[0] = offsets[3];
//       cal.gyro_zerorate[1] = offsets[4];
//       cal.gyro_zerorate[2] = offsets[5];
      
//       cal.mag_hardiron[0] = offsets[6];
//       cal.mag_hardiron[1] = offsets[7];
//       cal.mag_hardiron[2] = offsets[8];

//       cal.mag_field = offsets[9];
      
//       cal.mag_softiron[0] = offsets[10];
//       cal.mag_softiron[1] = offsets[13];
//       cal.mag_softiron[2] = offsets[14];
//       cal.mag_softiron[3] = offsets[13];
//       cal.mag_softiron[4] = offsets[11];
//       cal.mag_softiron[5] = offsets[15];
//       cal.mag_softiron[6] = offsets[14];
//       cal.mag_softiron[7] = offsets[15];
//       cal.mag_softiron[8] = offsets[12];

//       if (! cal.saveCalibration()) {
//         Serial.println("**WARNING** Couldn't save calibration");
//       } else {
//         Serial.println("Wrote calibration");    
//       }
//       cal.printSavedCalibration();
//       calcount = 0;
//       return;
//     }
//     // look for the 117,84 in the data, before discarding
//     for (i=2; i < 67; i++) {
//       if (caldata[i] == 117 && caldata[i+1] == 84) {
//         // found possible start within data
//         calcount = 68 - i;
//         memmove(caldata, caldata + i, calcount);
//         return;
//       }
//     }
//     // look for 117 in last byte
//     if (caldata[67] == 117) {
//       caldata[0] = 117;
//       calcount = 1;
//     } else {
//       calcount = 0;
//     }
//   }
// }


// uint16_t crc16_update(uint16_t crc, uint8_t a)
// {
//   int i;
//   crc ^= a;
//   for (i = 0; i < 8; i++) {
//     if (crc & 1) {
//       crc = (crc >> 1) ^ 0xA001;
//     } else {
//       crc = (crc >> 1);
//     }
//   }
//   return crc;
// }
