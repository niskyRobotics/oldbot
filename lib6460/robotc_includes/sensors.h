//////////////////////////////////////////////////////////////////////////////////////////
//
//                        Advanced Sensor Functions
//                        =========================
//
// As of April 2008, ROBOTC now contains basic drivers for many I2C sensor types directly
// implemented in the ROBOTC firmware. ROBOTC firmware will send appropriate initialization
// messages to the sensors and automatically periodically poll the sensor "value".
//
// The appropriate initialization message and the frequency of polling is internally
// stored as a data table within the ROBOTC firmware.
//
// The firmware contains sufficient intelligence to retrieve a single value for a sensor.
// For example:
//    - a single compositivbe color "index" from the HiTechnic sensor
//    - the first, or "X" axis, value from a triple axis accelerometer.
//
// This file contains a few routines that provide easy access to more data from the
// sensors.
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "I2CIntrinsics.h"

//#pragma platform(NXT)
#pragma systemFile      // this eliminates warning for "unreferenced" functions

//
// Get the Red, Green, Blue components from a HiTechnic Color Sensor
//
void getRGB(const tSensors nSensorIndex, int &nRed, int &nGreen, int &nBlue);

//
// Determine whether a color is "close" enough to the color detected by the HiTechnic
// color sensor. i.e. are all color components of the currently detected color within
// "nRange" of the specified color components
//
bool bColorMatched(const tSensors nSensorIndex, int nRed, int nGreen, int nBlue, int nRange);


//
// Get the X, y and Z components from a HiTechnic Accelerometer Sensor
//
void getAccel(const tSensors nSensorIndex, int &nXAxis, int &nYAxis, int &nZAxis);

//
// Find an I2C sensor with the specified manufacturer and sensor type
//
bool findSensor(tSensors& nSensor, const string& sVendor, const string& sSensorType);
