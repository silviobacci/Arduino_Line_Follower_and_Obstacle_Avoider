#ifndef infrared_h
#define infrared_h

// System modalities
enum system_mode {LINE_FOLLOWING, OBSTACLE_AVOIDING, LINE_SEARCHING};

// Direction useful to understand where is the line with respect to the car
enum dir {LEFT, CENTER, RIGHT, NO_LINE, FULL_LINE};

// Possible colours of the surface under the infrared sensor
enum colour {WHITE, BLACK, UNDEFINED};

//------------------------------------------------------------------------------
// CLASS DEFINITION
//------------------------------------------------------------------------------

class Infrared{
	//--------------------------------------------------------------------------
	// PRIVATE MEMBERS AND FUNCTIONS
	//--------------------------------------------------------------------------
	private:
		//----------------------------------------------------------------------
		// PRIVATE MEMBERS
		//----------------------------------------------------------------------

		int 			numSensors;		// Number of infrared sensors
		unsigned int *	pins;			// Board's pins attached to the sensors
		unsigned int *	sensorData;		// Data read from the sensors (0-1024)
		colour *		sensorColour;	// Colour seen by the sensors

		//----------------------------------------------------------------------
		// PRIVATE FUNCTIONS
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		// READ ANALOG PINS: Reads data from the infrared sensors
		//----------------------------------------------------------------------

		void readAnalogPins();

		//----------------------------------------------------------------------
		// DATA TO COLOUR: 	Convert the infrared data into the colour of the
		// 					surface seen by the sensor itself.
		//----------------------------------------------------------------------

		void dataToColour();

	//--------------------------------------------------------------------------
	// PUBLIC MEMBERS AND FUNCTIONS
	//--------------------------------------------------------------------------
	public:
		//----------------------------------------------------------------------
		// PUBLIC FUNCTIONS
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		// SENSOR ULTRASONIC: Constructors of the class
		//----------------------------------------------------------------------

		Infrared();
		Infrared(unsigned int *p, int ns);
		Infrared(int ns);

		//----------------------------------------------------------------------
		// TURNING DIRECTION: 	Returns the direction in which the car has to 
		// 						turn according to car's mode
		//----------------------------------------------------------------------

		dir turningDirection(int m);
};

#endif
