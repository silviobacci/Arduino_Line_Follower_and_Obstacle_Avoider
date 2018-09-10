#ifndef SensorUltrasonic_h
#define SensorUltrasonic_h

//------------------------------------------------------------------------------
// CLASS DEFINITION
//------------------------------------------------------------------------------

class SensorUltrasonic{
	//--------------------------------------------------------------------------
	// PRIVATE MEMBERS AND FUNCTIONS
	//--------------------------------------------------------------------------
    private:
		//----------------------------------------------------------------------
		// PRIVATE MEMBERS
		//----------------------------------------------------------------------

		int		trigPin;	// Board's PIN attached to the sensor's TRIGGER PIN
		int		echoPin;	// Board's PIN attached to the sensor's ECHO PIN
		long	timeout;	// Time after which we stop to listen the echoPin
		boolean detected;	// 1 if an obstacle was detected, 0 otherwise

		// Past values of the detcted distances
		long	currentDistance, previousDistance;
    
		//----------------------------------------------------------------------
		// PRIVATE FUNCTIONS
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		// TIMING: 	Sends the trigger signal and saves the duration of the echo
		// 			response
		//----------------------------------------------------------------------

		long	timing();

		//----------------------------------------------------------------------
		// READ DISTANCE: 	Using the timing function, converts the duration of
		// 					echo response in cm dividing for a specific costant
		//----------------------------------------------------------------------

		long	readDistance();

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

		SensorUltrasonic();
		SensorUltrasonic(int tp, int ep);
		SensorUltrasonic(int tp, int ep, long to);

		//----------------------------------------------------------------------
		// OBSTACLED DETECTED: 	Measures the distance from the obstacle and
		// 						returns true if the distance is less than a
		// 						specified threshold.
		//----------------------------------------------------------------------

		boolean obstacleDetected();

		//----------------------------------------------------------------------
		// DISTANCE FROM OBSTACLE: Returns the distance from the obstacle
		//----------------------------------------------------------------------

		int		distanceFromObstacle();
};

#endif
