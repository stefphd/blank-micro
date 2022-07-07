
#ifndef __LIBS_H__
#define __LIBS_H__

/*! \file libs.h
	\brief Include of libraries.
	\details File to include the necessary (external) library for objects. 
	\see objects.h
*/

#include <SPI.h> //for SPI communication (already included when installing teensyduino)
#include <ADC.h> //for ADC stuff https://github.com/pedvide/ADC (already included when installing teensyduino)
#include <ADC_util.h> //for ADC stuff https://github.com/pedvide/ADC (already included when installing teensyduino)
#include <T4_PowerButton.h> //for on/off button management https://github.com/FrankBoesing/T4_PowerButton/blob/master/examples/power/power.ino
#include <SD.h> //for saving in SD, includes SDFat
#include <controlModel.h> //include control model librariy (generated with the Embedeed coder)

#endif
