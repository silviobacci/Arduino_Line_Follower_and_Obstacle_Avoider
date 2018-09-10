//------------------------------------------------------------------------------
// INFRARED: Contains the library useful to manage the infrared sensors.
//------------------------------------------------------------------------------

#include <Arduino.h>
#include "infrared.h"

//------------------------------------------------------------------------------
// GLOBAL CONSTANTS
//------------------------------------------------------------------------------

#define DEFAULT_NUM_SENSORS		6		// Default number of infrared sensors
#define BLACK_THRESHOLD 		650		// Threshold useful to choose black
#define WHITE_THRESHOLD			350		// Threshold useful to choose white
#define N_CALIBRATIONS			100		// Number of calibrations to execute
#define DEFUALT_LED_PIN			13		// Predefined board's led pin

//------------------------------------------------------------------------------
// INFRARED: Constructors of the class
//------------------------------------------------------------------------------

Infrared::Infrared(unsigned int *p, int ns){
	numSensors = ns;
	sensorData = new unsigned int[numSensors];
	sensorColour = new colour[numSensors];
	pins = new unsigned int[numSensors];
	pins = p;
}

Infrared::Infrared(int ns){
	numSensors = ns;
	sensorData = new unsigned int[numSensors];
	sensorColour = new colour[numSensors];
	pins = new unsigned int[numSensors];
	for (int i = 0; i < numSensors ;i++)
		pins[i] = i;
}

Infrared::Infrared(){
	numSensors = DEFAULT_NUM_SENSORS;
	sensorData = new unsigned int[numSensors];
	sensorColour = new colour[numSensors];
	pins = new unsigned int[numSensors];
	for (int i = 0; i < numSensors ;i++)
		pins[i] = i;
}

//------------------------------------------------------------------------------
// READ ANALOG PINS: Reads data from the infrared sensors
//------------------------------------------------------------------------------

void Infrared::readAnalogPins(){
	for (int i = 0; i < numSensors ;i++)
		sensorData[i] = analogRead(pins[i]);
}

//------------------------------------------------------------------------------
// DATA TO COLOUR:	Convert the infrared data into the colour of the surface
// 					seen by the sensor itself.
//------------------------------------------------------------------------------

void Infrared::dataToColour(){
	for (int i = 0; i < numSensors; i++) {
		if (sensorData[i] < WHITE_THRESHOLD)
			sensorColour[i] = WHITE;
		else{
			if (sensorData[i] > BLACK_THRESHOLD)
				sensorColour[i] = BLACK;
			else
				sensorColour[i] = UNDEFINED;
		}
	}
}

//------------------------------------------------------------------------------
// TURNING DIRECTION: 	Returns the direction in which the car has to turn 
// 						according to car's mode
//------------------------------------------------------------------------------

dir Infrared::turningDirection(int m){
	readAnalogPins();
	dataToColour();

	// According to the position of the line we choose the direction to turn
	if ((sensorColour[0] == BLACK || sensorColour[1] == BLACK) && (sensorColour[4] == BLACK || sensorColour[5] == BLACK))
		return FULL_LINE;

	if (sensorColour[4] == BLACK || sensorColour[5] == BLACK)
		return RIGHT;
	if (sensorColour[0] == BLACK || sensorColour[1] == BLACK)
		return LEFT;

	switch(m){
		case LINE_FOLLOWING:
			if(sensorColour[2] == BLACK && sensorColour[3] == BLACK)
				return CENTER;
		default:
			if(sensorColour[2] == BLACK || sensorColour[3] == BLACK)
				return CENTER;
	}
    
	return NO_LINE;
}