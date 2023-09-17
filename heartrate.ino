/*
Heart Rate Module for Ansieta
*/
#include "DFRobot_Heartrate.h"

DFRobot_Heartrate heartrate(DIGITAL_MODE); ///< ANALOG_MODE or DIGITAL_MODE

int getHeartRate() {
  /*
  Returns the heart rate.
  The library uses 10 consecutive proper readings which it averages.
  If the heart rate is not available, return 0.
  */
  uint8_t ratevalue = heartrate.getRate();
  if(ratevalue)  {
    Serial.println(ratevalue);
  }
  return ratevalue;
}
void monitorHeartRate() {
  /*
  Update the heart rate state.
  */
  heartrate.getValue(HEARTRATE_PIN);
}