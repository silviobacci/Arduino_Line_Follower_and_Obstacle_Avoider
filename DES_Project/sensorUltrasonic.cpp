//------------------------------------------------------------------------------
// SENSOR ULTRASONIC: Contain the library useful to manage the ultrasonic sensor
//------------------------------------------------------------------------------
#include <Arduino.h>
#include "sensorUltrasonic.h"

//------------------------------------------------------------------------------
// GLOBAL CONSTANTS
//------------------------------------------------------------------------------

#define	DEFAULT_DISTANCE	15		// Threshold for the obstacle distance
#define DEFAULT_TIMEOUT		3000	// 3000 µs = 50cm // 30000 µs = 5 m
#define DEFAULT_TRIG_PIN	8		// Predefined board's trigger pin
#define DEFAULT_ECHO_PIN	7		// Predefined board's echo pin
#define MICROSEC_TO_CM		58		// Costant to convert "duration" -> cm

//------------------------------------------------------------------------------
// SENSOR ULTRASONIC: Constructors of the class
//------------------------------------------------------------------------------

SensorUltrasonic::SensorUltrasonic(int tp, int ep, long to){
	pinMode(tp,OUTPUT);
	pinMode(ep,INPUT);
	trigPin = tp;
	echoPin = ep;
	timeout = to;
}

SensorUltrasonic::SensorUltrasonic(int tp, int ep){
	pinMode(tp,OUTPUT);
	pinMode(ep,INPUT);
	trigPin = tp;
	echoPin = ep;
	timeout = DEFAULT_TIMEOUT;
}

SensorUltrasonic::SensorUltrasonic(){
	pinMode(DEFAULT_TRIG_PIN,OUTPUT);
	pinMode(DEFAULT_ECHO_PIN,INPUT);
	trigPin = DEFAULT_TRIG_PIN;
	echoPin = DEFAULT_ECHO_PIN;
	timeout = DEFAULT_TIMEOUT;
}

//------------------------------------------------------------------------------
// TIMING: 	Sends the trigger signal and saves the duration of the echo
// 			response
//------------------------------------------------------------------------------

long SensorUltrasonic::timing(){
	// We ensure to have a 0 trigger signal
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);

	// We put to 1 the trigger signal for 10 us in order to start the ranging
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);

	// We put to 0 the trigger signal
	digitalWrite(trigPin, LOW);

	// We count the time duration of the 1 in the echo signal up to timeout
	long duration = pulseIn(echoPin, HIGH, timeout);

	if (duration == 0)
		duration = timeout;

	return duration;
}

//------------------------------------------------------------------------------
// READ DISTANCE: 	Using the timing function, converts the duration of
// 					echo response in cm dividing for a specific costant
//------------------------------------------------------------------------------

long SensorUltrasonic::readDistance(){
	return timing() / MICROSEC_TO_CM;
}

//------------------------------------------------------------------------------
// OBSTACLED DETECTED: 	Measures the distance from the obstacle and
// 						returns true if the distance is less than a
// 						specified threshold.
//------------------------------------------------------------------------------

boolean SensorUltrasonic::obstacleDetected(){
	// We use an weighted sum in order to filter the distances measured
	long newDistance = 0.6 * readDistance() + 0.3 * currentDistance + 0.1 * previousDistance;
	previousDistance = currentDistance;
	currentDistance = newDistance;

	if (currentDistance <= DEFAULT_DISTANCE)
		detected = true;
	else
		detected = false;

	return detected;
}

//------------------------------------------------------------------------------
// DISTANCE FROM OBSTACLE: Returns the distance from the obstacle
//------------------------------------------------------------------------------

int SensorUltrasonic::distanceFromObstacle(){
	return (int) currentDistance;
}