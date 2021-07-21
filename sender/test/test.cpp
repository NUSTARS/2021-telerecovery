#include "ToF.h"

Adafruit_VL53L0X ToF = Adafruit_VL53L0X();

void setup(){
    Serial.begin(115200);
    
    // wait until serial port opens for native USB devices
    while (! Serial) {
    delay(1);
    }

    // Initialize time of flight sensor
    if (!ToF.begin()) {
        Serial.println(F("Failed to boot VL53L0X"));
        while(1);
    }

}


void loop(){
    VL53L0X_RangingMeasurementData_t val = read_ToF(ToF);

    if (val.RangeStatus != 4) {  // phase failures have incorrect data
        Serial.print("Distance (mm): "); Serial.println(val.RangeMilliMeter);
    } 
    else {
        Serial.println(" out of range ");
    }

    delay(100);
}