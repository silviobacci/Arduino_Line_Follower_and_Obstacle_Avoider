#ifndef CAR_H
#define CAR_H

#include <Servo.h>

//------------------------------------------------------------------------------
// GLOBAL CONSTANTS
//------------------------------------------------------------------------------

#define RIGHT_SERVO_PIN	10 	// Default board's pin attached to the right servo
#define LEFT_SERVO_PIN 	11 	// Default board's pin attached to the left servo

// Possible speeds the car can reach
enum velocity {STOP, LOW_SPEED, MEDIUM_SPEED, HIGH_SPEED, MAXIMUM_SPEED};

//------------------------------------------------------------------------------
// CLASS DEFINITION
//------------------------------------------------------------------------------
  
class Car{
	//--------------------------------------------------------------------------
	// PRIVATE MEMBERS AND FUNCTIONS
	//--------------------------------------------------------------------------
	private:
		//----------------------------------------------------------------------
		// PRIVATE MEMBERS
		//----------------------------------------------------------------------

		Servo rightServo;		// Car's left servo
		Servo leftServo;		// Car's right servo
		int speed;				// Car's speed in cm/s
		double angularSpeed;	// Car's angular speed in rad/s
		velocity vel;			// Car's velocity in terms of modality
		int microSeconds;		// PWM width sent to the servo

		//----------------------------------------------------------------------
		// PRIVATE FUNCTIONS
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		// SET SPEED PARAM: According to the chosen velocity v, the function
		// 					saves all the useful paramters like speed, angular
		// 					speed and so on.
		//----------------------------------------------------------------------

		void setSpeedParam(velocity v);

	//--------------------------------------------------------------------------
	// PUBLIC MEMBERS AND FUNCTIONS
	//--------------------------------------------------------------------------
	public:
		//----------------------------------------------------------------------
		// PUBLIC FUNCTIONS
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		// SENSOR ULTRASONIC: Constructor of the class
		//----------------------------------------------------------------------

		Car(int rightServoPin = RIGHT_SERVO_PIN, int leftServoPin = LEFT_SERVO_PIN, velocity v = STOP);

		//----------------------------------------------------------------------
		// SET VELOCITY: Set car's speed in terms of cm/s
		//----------------------------------------------------------------------

		void setVelocity(velocity v = MAXIMUM_SPEED);

		//----------------------------------------------------------------------
		// GET VELOCITY: Returns car's speed in terms of cm/s
		//----------------------------------------------------------------------

		int  getVelocity();
		
		//----------------------------------------------------------------------
		// GET SPEED MODE: Returns car's speed in terms of modality
		//----------------------------------------------------------------------

		int  getSpeedMode();

		//----------------------------------------------------------------------
		// GET ANGULAR VELOCITY: Returns car's angular speed in terms of rad/s
		//----------------------------------------------------------------------

		double getAngularVelocity();

		//----------------------------------------------------------------------
		// TURN LEFT: 	Stop the left servo in order to turn on the left while 
		// 				the right servo keeps the velocity v.
		//----------------------------------------------------------------------

		void turnLeft(velocity v = MAXIMUM_SPEED);

		//----------------------------------------------------------------------
		// TURN RIGHT: 	Stop the right servo in order to turn on the right while
		// 				the left servo keeps the velocity v.
		//----------------------------------------------------------------------

		void turnRight(velocity v = MAXIMUM_SPEED);

		//----------------------------------------------------------------------
		// GO STRAIGHT: Set the servos' speed to the proper velocity in order to
		// 				go straight on.
		//----------------------------------------------------------------------

		void goStraight(velocity v = MAXIMUM_SPEED);
};

#endif
