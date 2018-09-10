//------------------------------------------------------------------------------
// DES PROJECT:	Contains the logic of the system, the initialization and the
// 				task periodically executed
//------------------------------------------------------------------------------

#include "car.h"
#include "infrared.h"
#include "sensorUltrasonic.h"
#include "timer.h"

//------------------------------------------------------------------------------
// GLOBAL CONSTANTS
//------------------------------------------------------------------------------

#define L					6	// Half width of the car
#define l					3	// Half width of the obstacle
#define d					6	// Distance between wheels and infrareds
#define SAFETY_ANGLE		20	// Angle useful to avoid the obstacle
#define DEFUALT_LED_PIN		13	// Predefined board's led pin

// Steps to follow in order to avoid an obstacle
enum avoiding_steps {TURN_TO_AVOID, GO_ON_TO_AVOID, TURN_TO_LINE, GO_ON_TO_LINE};

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

// Instance of the class that manages the motors
Car  myCar = Car();

// Instance of the class that manages the timer
Timer timer = Timer();

// Instance of the class that manages the infrared sensors
Infrared infrared = Infrared();

// Instance of the class that manages the ultrasonic sensor
SensorUltrasonic ultrasonic = SensorUltrasonic();

// Delays useful to avoid the obstacles
int T1, T2;		

// Saves the direction of the travel								
dir directionToAvoidObstacle, firstDirection; 

// Bool useful to say if we have to consider the line when we are centered on it
boolean lookCenter;

// Boolean useful to say if an obstacle was detected or not
boolean obstacleDetected;

// Current system modality
system_mode mode;

// Current step to avoid the obstacle
avoiding_steps avoidingStep;

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// SET AVOIDING PARAM: 	Set all the parameters useful to manage the obstacle
//						avoidance
//------------------------------------------------------------------------------

void setAvoidingParam(){
	int D = ultrasonic.distanceFromObstacle() - 1;
	double beta = PI/2 - atan2(D,L+l) + SAFETY_ANGLE * DEG_TO_RAD;
	T1 = (int) (beta / myCar.getAngularVelocity() * 1000);
	double safety_distance = (d + D + 2*l - 2*L*sin(beta))/cos(beta);
	T2 = (int) (safety_distance / myCar.getVelocity() * 1000);
}

//------------------------------------------------------------------------------
// TO OBSTACLE AVOIDING: 	Perform all the operations useful to start avoiding
//							the obstacle (first operation is turning into the
//							center of the circuit)
//------------------------------------------------------------------------------

void toObstacleAvoiding(){
	digitalWrite(DEFUALT_LED_PIN, HIGH);
	mode = OBSTACLE_AVOIDING;
	directionToAvoidObstacle = firstDirection;
	firstDirection = NO_LINE;
	setAvoidingParam();

	// According to the direction of travel we go into the center of the circuit
	if (directionToAvoidObstacle == LEFT)
		myCar.turnLeft();
	else
		myCar.turnRight();

	// Starts to turn for a time equal to T1
	timer.start(T1);

	avoidingStep = TURN_TO_AVOID;
}

//------------------------------------------------------------------------------
// TURN AVOID: 	Checks if the car has turned enough and starts to go straight
//				searching the line
//------------------------------------------------------------------------------

void turnToAvoid(){
	//--------------------------------------------------------------------------
	// Checks if the time T1 was reached in order to go to the next step,
	// otherwise we have to check if there is the line
	//--------------------------------------------------------------------------
	if (timer.isTimeoutReached()){
		avoidingStep = GO_ON_TO_AVOID;

		myCar.goStraight();

		// Starts to go straight for a time equal to T2
		timer.start(T2);
	}
}

//------------------------------------------------------------------------------
// GO IN TO AVOID: 	Checks if the car has gone on enough and starts to come back
//					on the black line searching the line and keeeping attention
//					to other obstacles
//------------------------------------------------------------------------------

void goOnToAvoid(){
	//--------------------------------------------------------------------------
	// Checks if the time T2 was reached in order to go to the next step,
	// otherwise we have to check if there is the line or a new obstacle
	//--------------------------------------------------------------------------
	if (timer.isTimeoutReached()){
		avoidingStep = TURN_TO_LINE;

		if(directionToAvoidObstacle == LEFT)
			myCar.turnRight();
		else
			myCar.turnLeft();

		// Starts to go back to the line for a time equal to 2*T1
		timer.start(2 * T1);
	}
	else{
		if (obstacleDetected)
			toObstacleAvoiding();
		else
			searchLine();
	}
}

//------------------------------------------------------------------------------
// TURN AVOID: 	Checks if the car has turned enough and starts to go straight
//				searching the line and keeping attention to other obstacles
//------------------------------------------------------------------------------

void turnToLine(){
	//--------------------------------------------------------------------------
	// Checks if the time 2*T1 was reached in order to go to the next step,
	// otherwise we have to check if there is the line or a new obstacle
	//--------------------------------------------------------------------------
	if (timer.isTimeoutReached()){
		avoidingStep = GO_ON_TO_LINE;
		myCar.goStraight();
	}
	else{
		if (obstacleDetected)
			toObstacleAvoiding();
		else
			searchLine();
	}
}

//------------------------------------------------------------------------------
// AVOID OBSTACLE: Performs all the steps useful to avoid the obstacle
//------------------------------------------------------------------------------

void avoidObstalce(){
	switch(avoidingStep){
		case TURN_TO_AVOID:
			turnToAvoid();
			break;

		case GO_ON_TO_AVOID:
			goOnToAvoid();
			break;

		case TURN_TO_LINE:
			turnToLine();
			break;

		case GO_ON_TO_LINE:
			mode = LINE_SEARCHING;
			break;

		default:
			break;
	}
}

//------------------------------------------------------------------------------
// SEARCH LINE: Checks if the line was found or not and changes car's behaviour
//				in order to start to follow the line once found it
//------------------------------------------------------------------------------

void searchLine(){
	// Get data from infrared sensors
	dir newDirection = infrared.turningDirection(mode);

	// According to the read data we change car's behaviour
	switch(newDirection){
		case LEFT:
			if (firstDirection == NO_LINE)
				firstDirection = RIGHT;
			break;

		case RIGHT:
			if (firstDirection == NO_LINE)
				firstDirection = LEFT;
			break;

		case CENTER:
			mode = LINE_FOLLOWING;
			if (firstDirection == RIGHT)
				myCar.turnRight();
			else if (firstDirection == LEFT)
				myCar.turnLeft();
			digitalWrite(DEFUALT_LED_PIN, LOW);
			break;

		case FULL_LINE:
			switch(firstDirection){
				case LEFT:
					myCar.turnLeft();
					break;

				case RIGHT:
					myCar.turnRight();
					break;

				default:
					myCar.turnRight();
					firstDirection = RIGHT;
					break;
			}
			lookCenter = false;
			break;

		default:
			break;
	}
}

//------------------------------------------------------------------------------
// FOLLOW LINE: According to the position of the line, changes car's behaviour
//------------------------------------------------------------------------------

void followLine(){
	// Get data from infrared sensors
	dir newDirection = infrared.turningDirection(mode);

	switch(newDirection){
		case RIGHT:
			myCar.turnRight();
			if (lookCenter == false)
				lookCenter = true;
			break;

		case LEFT:
			myCar.turnLeft();
			if (lookCenter == false)
				lookCenter = true;
			break;

		case CENTER:
			if (lookCenter == true)
				myCar.goStraight();
			break;

		default:
			break;
  }
}

//------------------------------------------------------------------------------
// SETUP: Initialize all the needed variables and turns on the LED
//------------------------------------------------------------------------------

void setup() {
	mode = LINE_SEARCHING;
	obstacleDetected = false;
	lookCenter = true;
	directionToAvoidObstacle = firstDirection = NO_LINE;

	// Delay to be sure everything has been set properly
	delay(1000);

	digitalWrite(DEFUALT_LED_PIN, HIGH);
	myCar.setVelocity();
}

//------------------------------------------------------------------------------
// LOOP:	Checks the presence of an obstacle and according to the modality
//			changes the car's behaviour in order to both follow the black line
//			and avoid the obstacles
//------------------------------------------------------------------------------

void loop() {
	obstacleDetected = ultrasonic.obstacleDetected();

	switch(mode){
		case LINE_SEARCHING:
			if(obstacleDetected && myCar.getSpeedMode() != STOP)
				toObstacleAvoiding();
			else
				searchLine();
			break;

		case LINE_FOLLOWING:
			if(obstacleDetected)
				toObstacleAvoiding();
			else
				followLine();
			break;

		case OBSTACLE_AVOIDING:
			avoidObstalce();
			break;

		default:
			break;
	}
}
