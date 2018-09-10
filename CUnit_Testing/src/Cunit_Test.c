//------------------------------------------------------------------------------
// CUNIT TEST: Contain 3 tests to be performed on 3 different functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// COMPILE COMMAND: gcc Cunit_Test.c -lcunit -o CTest -lm
//------------------------------------------------------------------------------

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CUnit/Basic.h"

//------------------------------------------------------------------------------
// GLOBAL CONSTANTS
//------------------------------------------------------------------------------

#define DEFAULT_NUM_SENSORS		6
#define L						6
#define l						3
#define d						6
#define NUM_SPEEDS				5

#define DEG_TO_RAD				M_PI/180
#define RAD_TO_DEG				180/M_PI

#define SAFETY_ANGLE			20 * DEG_TO_RAD
#define LOW_ANGULAR_SPEED		22 * DEG_TO_RAD
#define MEDIUM_ANGULAR_SPEED	44 * DEG_TO_RAD
#define HIGH_ANGULAR_SPEED		74 * DEG_TO_RAD
#define MAX_ANGULAR_SPEED		85 * DEG_TO_RAD

#define LOW_CMS_SPEED			5
#define MEDIUM_CMS_SPEED		10
#define HIGH_CMS_SPEED			15
#define MAX_CMS_SPEED			17

#define AVOID_TEST_NUM			15
#define TURNING_TEST_NUM		128
#define SEARCH_TEST_NUM			16

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// TURNING DIRECTION TESTING VARIABLES
//------------------------------------------------------------------------------

enum system_mode {LINE_FOLLOWING, OBSTACLE_AVOIDING, LINE_SEARCHING};
typedef enum dir {LEFT, CENTER, RIGHT, NO_LINE, FULL_LINE} dir;
typedef enum colour {WHITE, BLACK, UNDEFINED} colour;
colour sensorColour[DEFAULT_NUM_SENSORS];
colour TsensorColour[TURNING_TEST_NUM][DEFAULT_NUM_SENSORS];
int m[TURNING_TEST_NUM], results[TURNING_TEST_NUM];

//------------------------------------------------------------------------------
// SET AVOID PARAM TESTING VARIABLES
//------------------------------------------------------------------------------

enum velocity {STOP, LOW_SPEED, MEDIUM_SPEED, HIGH_SPEED, MAXIMUM_SPEED} velocity;
int T1, T2, D, speed;
double beta, safety_distance;

//------------------------------------------------------------------------------
// SEARCH LINE TESTING VARIABLES
//------------------------------------------------------------------------------

int mode, directionToAvoidObstacle[SEARCH_TEST_NUM], newDirection[SEARCH_TEST_NUM];
int i = 0, j = 0;

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// INIT SUITE1: Initialization function for turning direction test
//------------------------------------------------------------------------------

int init_suite1(void) {
    FILE * fp;
    char * line = NULL;
    char * token = NULL;
    size_t len = 0;
	unsigned int mask = 1U << (DEFAULT_NUM_SENSORS-1);
	int in, i = 0, j = 0;
    
    for(i = 0; i < TURNING_TEST_NUM; i++){
        if(i < TURNING_TEST_NUM/2){
            m[i] = 0;
            in = i;
        }
        else{
            m[i] = 1;
            in = i - TURNING_TEST_NUM/2;
        }
        for (j = 0; j < DEFAULT_NUM_SENSORS; j++) {
            TsensorColour[i][j] = (in & mask) ? 1 : 0;
            in <<= 1;
        }
    }
    
    i = 0;

	// Getting output data
	fp = fopen("./TurningDirectionOutput.txt", "r");

	while ((getline(&line, &len, fp)) != -1) {
		token = strtok(line,"\rn");
        
		if (strcmp(token,"LEFT") == 0)
			results[i++] = LEFT;
		else if (strcmp(token,"RIGHT") == 0)
			results[i++] = RIGHT;
		else if (strcmp(token,"FULL_LINE") == 0)
			results[i++] = FULL_LINE;
		else if (strcmp(token,"NO_LINE") == 0)
			results[i++] = NO_LINE;
		else
			results[i++] = CENTER;
	}

	fclose(fp);
    
	return 0;
}

//------------------------------------------------------------------------------
// TURNING DIRECTION: Function to test
//------------------------------------------------------------------------------

dir turningDirection(int m) {
	if((sensorColour[0] == BLACK || sensorColour[1] == BLACK) && (sensorColour[4] == BLACK || sensorColour[5] == BLACK))
		return FULL_LINE;
	if(sensorColour[4] == BLACK || sensorColour[5] == BLACK)
		return RIGHT;
	if(sensorColour[0] == BLACK || sensorColour[1] == BLACK)
		return LEFT;

	switch(m){
		case LINE_FOLLOWING:
			if(sensorColour[2] == BLACK || sensorColour[3] == BLACK)
				return CENTER;
		default:
			if(sensorColour[2] == BLACK && sensorColour[3] == BLACK)
				return CENTER;
	}

	return NO_LINE;
}

//------------------------------------------------------------------------------
// TEST TURNING DIRECTION: Contains all the tests to be performed
//------------------------------------------------------------------------------

void testTurningDirection(void) {
	for (i = 0; i < TURNING_TEST_NUM; i++) {
		for (j = 0; j < DEFAULT_NUM_SENSORS; j++)
			sensorColour[j] = TsensorColour[i][j];
		CU_ASSERT(results[i] == turningDirection(m[i]));
	}
}

//------------------------------------------------------------------------------
// CLEAN SUITE1: Clean function for turning direction test
//------------------------------------------------------------------------------

int clean_suite1(void) {
	return 0;
}


//------------------------------------------------------------------------------
// INIT SUITE2: Initialization function for set avoiding param test
//------------------------------------------------------------------------------

int init_suite2(void) {
	return 0;
}

//------------------------------------------------------------------------------
// CAR ANGULAR SPEED: Auxiliary function
//------------------------------------------------------------------------------

double carAngularSpeed(int speed) {
	switch(speed){
		case LOW_SPEED:
			return LOW_ANGULAR_SPEED;
			
		case MEDIUM_SPEED:
			return MEDIUM_ANGULAR_SPEED;
			
		case HIGH_SPEED:
			return HIGH_ANGULAR_SPEED;
			
		case MAXIMUM_SPEED:
			return MAX_ANGULAR_SPEED;
			
		default:
			return 0;
			break;
		
	}
}

//-------------------------------------------------------------------------------
// CAR SPEED: Auxiliary function
//-------------------------------------------------------------------------------

double carSpeed(int speed) {
	switch(speed){
		case LOW_SPEED:
			return LOW_CMS_SPEED;
			
		case MEDIUM_SPEED:
			return MEDIUM_CMS_SPEED;
			
		case HIGH_SPEED:
			return HIGH_CMS_SPEED;
			
		case MAXIMUM_SPEED:
			return MAX_CMS_SPEED;
			
		default:
			return 0;
			break;
	}
}

//------------------------------------------------------------------------------
// SET AVOIDING PARAM: Function to test
//------------------------------------------------------------------------------

void setAvoidingParam() {
	beta = M_PI/2 - atan2(D,L+l) + SAFETY_ANGLE;
	T1 = (int) (beta / ((double) carAngularSpeed(speed)) * 1000);
	safety_distance = (d + D + 2*l - 2*L*sin(beta))/cos(beta);
	T2 = (int) (safety_distance /  carSpeed(speed) * 1000);
}

//------------------------------------------------------------------------------
// TEST AVOIDING PARAM: Contains all the tests to be performed
//------------------------------------------------------------------------------

void testSetAvoidingParam(void) {
	for (j = 1; j < NUM_SPEEDS ; j++) {
		speed = j;
		for (i = AVOID_TEST_NUM; i >= 0; i--) {
			D = i;
			setAvoidingParam();
			CU_ASSERT(50 <= beta * RAD_TO_DEG && beta * RAD_TO_DEG <= 110);
			CU_ASSERT_DOUBLE_EQUAL(beta, carAngularSpeed(speed) * ((double) T1) / 1000, beta / 100);
			CU_ASSERT_DOUBLE_EQUAL(safety_distance, carSpeed(speed) * ((double) T2) / 1000, safety_distance / 100);
		}
	}
}

//------------------------------------------------------------------------------
// CLEAN SUITE2: Clean function for set avoiding param test
//------------------------------------------------------------------------------

int clean_suite2(void) {
	return 0;	
}

//------------------------------------------------------------------------------
// INIT SUITE3: Initialization function for serach line test
//------------------------------------------------------------------------------

int init_suite3(void) {
    for(i = 0; i < 5; i++){
        for(j = 0; j < 5; j++){
            newDirection[i+j] = i;
            directionToAvoidObstacle[i+j] = j;
        }
    }
    
	return 0;
}

//------------------------------------------------------------------------------
// SEARCH LINE: Function to test
//------------------------------------------------------------------------------

void searchLine() {
	switch(newDirection[i]){
		case CENTER:
			mode = LINE_FOLLOWING;
			//digitalWrite(LED_PIN, LOW);
			break;
		case FULL_LINE:
			//myCar.turnRight();
			if(directionToAvoidObstacle[i] == NO_LINE)
				directionToAvoidObstacle[i] = RIGHT;
			break;
		default:
			break;
	}
}

//------------------------------------------------------------------------------
// TEST SEARCH LINE: Contains all the tests to be performed
//------------------------------------------------------------------------------

void testSearchLine(void){
    int dirAvoid, newDir;
	for (i = 0; i < SEARCH_TEST_NUM ; i++){
        mode = LINE_SEARCHING;
        dirAvoid = directionToAvoidObstacle[i];
        newDir = newDirection[i];
		searchLine();

		if (newDir == CENTER){
			CU_ASSERT(mode == LINE_FOLLOWING);
		}
        
        if(newDir == FULL_LINE && dirAvoid == NO_LINE){
			CU_ASSERT(directionToAvoidObstacle[i] == RIGHT);
		}
	}
}

//------------------------------------------------------------------------------
// CLEAN SUITE3: Clean function for search line test
//------------------------------------------------------------------------------

int clean_suite3(void){
	return 0;
}

//------------------------------------------------------------------------------
// MAIN: Tests execution
//------------------------------------------------------------------------------

int main() {
	CU_pSuite pSuite = NULL;

	// Initialize the CUnit test registry
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	// Add suite1 to the registry
	pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
	if (pSuite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	// Add the tests to the suite1
	if (CU_add_test(pSuite, "Test of turningDirection()", testTurningDirection) == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	// Add suite2 to the registry
	pSuite = CU_add_suite("Suite_2", init_suite2, clean_suite2);
	if (pSuite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	// Add the tests to the suite2
	if (CU_add_test(pSuite, "Test of setAvoidingParam()", testSetAvoidingParam) == NULL){
		CU_cleanup_registry();
		return CU_get_error();
	}

	// Add suite3 to the registry
	pSuite = CU_add_suite("Suite_3", init_suite3, clean_suite3);
	if (pSuite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}
   
	// Add the tests to the suite2
	if (CU_add_test(pSuite, "Test of SearchLine()", testSearchLine) == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	// Run all tests using the CUnit Basic interface
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
