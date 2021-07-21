#include "ToF.h"

VL53L0X_RangingMeasurementData_t read_ToF(Adafruit_VL53L0X ToF){

  VL53L0X_RangingMeasurementData_t val; // Initialize data variable

  ToF.rangingTest(&val, false); // pass in 'true' to get debug data printout!

  return val;
}