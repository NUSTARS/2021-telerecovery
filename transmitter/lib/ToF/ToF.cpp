#include "ToF.h"


// read_ToF() reads the time of flight sensor and returns the range measurement data object as 
// defined in the Adafruit_VL53L0X library
VL53L0X_RangingMeasurementData_t read_ToF(Adafruit_VL53L0X ToF){

  VL53L0X_RangingMeasurementData_t val; // Initialize data variable

  ToF.rangingTest(&val, false); // pass in 'true' to get debug data printout!

  return val;
}