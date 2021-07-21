#include "ToF.h"

// void init_ToF(Adafruit_VL53L0X ToF){

//     if (!ToF.begin()) {
//       Serial.println(F("Failed to boot VL53L0X"));
//       while(1);
//     }

// }


VL53L0X_RangingMeasurementData_t read_ToF(Adafruit_VL53L0X ToF){

  VL53L0X_RangingMeasurementData_t val;
    
  Serial.print("Reading a measurement... ");
  ToF.rangingTest(&val, false); // pass in 'true' to get debug data printout!

  return val;
}