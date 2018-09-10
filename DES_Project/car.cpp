//------------------------------------------------------------------------------
// CAR:  Contains car's behavior functions. 
//------------------------------------------------------------------------------

#include <Arduino.h>
#include "car.h"

//------------------------------------------------------------------------------
// GLOBAL CONSTANTS
//------------------------------------------------------------------------------

#define ZERO_SPEED_MICRO		1500	// 0 speed according to pwm params

#define LOW_MICRO_SPEED			25		// Lowest speed according to pwm params
#define MEDIUM_MICRO_SPEED		50		// Default speed according to pwm params
#define HIGH_MICRO_SPEED		90		// High speed according to pwm params
#define MAX_MICRO_SPEED			200		// Highest speed according to pwm params

#define LOW_CMS_SPEED			5		// Lowest speed (cm/s)
#define MEDIUM_CMS_SPEED		10		// Defualt speed (cm/s)
#define HIGH_CMS_SPEED			15		// High speed (cm/s)
#define MAX_CMS_SPEED			17		// Highest speed (cm/s)

#define LOW_ANGULAR_SPEED		22 * DEG_TO_RAD	// Lowest angular speed (rad/s)
#define MEDIUM_ANGULAR_SPEED	44 * DEG_TO_RAD	// Default angular speed (rad/s)
#define HIGH_ANGULAR_SPEED		74 * DEG_TO_RAD	// High angular speed (rad/s)
#define MAX_ANGULAR_SPEED		85 * DEG_TO_RAD	// Highest angular speed (rad/s)

//------------------------------------------------------------------------------
// CAR: Constructor of the class
//------------------------------------------------------------------------------

Car::Car(int rightServoPin, int leftServoPin, velocity v) {
	rightServo.attach(rightServoPin);
	leftServo.attach(leftServoPin);
	setVelocity(v);
}

//------------------------------------------------------------------------------
// SET SPEED PARAM: According to the chosen velocity v, the function saves all
// 					the useful paramters like speed, angular speed and so on.
//------------------------------------------------------------------------------

void Car::setSpeedParam(velocity v){
	vel = v;

	switch (vel) {
		case STOP:
			angularSpeed = 0;
			speed = 0;
			microSeconds = 0;
			break;
			
		case LOW_SPEED:
			angularSpeed = LOW_ANGULAR_SPEED;
			speed = LOW_CMS_SPEED;
			microSeconds = LOW_MICRO_SPEED;
			break;
			
		case MEDIUM_SPEED:
			angularSpeed = MEDIUM_ANGULAR_SPEED;
			speed = MEDIUM_CMS_SPEED;
			microSeconds = MEDIUM_MICRO_SPEED;
			break;
			
		case HIGH_SPEED:
			angularSpeed = HIGH_ANGULAR_SPEED;
			speed = HIGH_CMS_SPEED;
			microSeconds = HIGH_MICRO_SPEED;
			break;
			
		case MAXIMUM_SPEED:
			angularSpeed = MAX_ANGULAR_SPEED;
			speed = MAX_CMS_SPEED;
			microSeconds = MAX_MICRO_SPEED;
			break;
		default:
			break;
		}
}

//------------------------------------------------------------------------------
// SET VELOCITY: Set car's speed in terms of cm/s
//------------------------------------------------------------------------------

void Car::setVelocity(velocity v) {
	setSpeedParam(v);

	leftServo.writeMicroseconds(ZERO_SPEED_MICRO + microSeconds);
	rightServo.writeMicroseconds(ZERO_SPEED_MICRO - microSeconds);
}

//------------------------------------------------------------------------------
// GET VELOCITY: Returns car's speed in terms of cm/s
//------------------------------------------------------------------------------

int Car::getVelocity() {
	return speed;
}

//----------------------------------------------------------------------
// GET SPEED MODE: Returns car's speed in terms of modality
//----------------------------------------------------------------------

int Car::getSpeedMode() {
	return vel;
}

//------------------------------------------------------------------------------
// GET ANGULAR VELOCITY: Returns car's angular speed in terms of rad/s
//------------------------------------------------------------------------------

double Car::getAngularVelocity() {
	return angularSpeed;
}

//------------------------------------------------------------------------------
// TURN LEFT: 	Stop the left servo in order to turn on the left while the right
// 				servo keeps the velocity v.
//------------------------------------------------------------------------------

void Car::turnLeft(velocity v) {
	if(v != vel)
		setSpeedParam(v);

	leftServo.writeMicroseconds(ZERO_SPEED_MICRO);
	rightServo.writeMicroseconds(ZERO_SPEED_MICRO - microSeconds);
}

//------------------------------------------------------------------------------
// TURN RIGHT: 	Stop the right servo in order to turn on the right while the 
// 				left servo keeps the velocity v.
//------------------------------------------------------------------------------

void Car::turnRight(velocity v) {
	if(v != vel)
		setSpeedParam(v);

	leftServo.writeMicroseconds(ZERO_SPEED_MICRO + microSeconds);
	rightServo.writeMicroseconds(ZERO_SPEED_MICRO);
}

//------------------------------------------------------------------------------
// GO STRAIGHT: Set the servos' speed to the proper velocity in order to go 
// 				straight on.
//------------------------------------------------------------------------------

void Car::goStraight(velocity v){
	setVelocity(v);
}