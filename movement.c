/*	movement.c *
*	This is going to be written without any bugs 
*/

//#include "GPS/gps.h"

#define _BSD_SOURCE
#include <string.h>
#include "gpio.h"
#include <unistd.h>
#include "i2c-dev.h"
#include "Compass/compass.h" // booooooo
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include "Waypoints/waypoint.h"
#include "GPSLib/gps.h"
#include "movement.h"

#define LEFT_BCK_GPIO 71
#define LEFT_FWD_GPIO 75
#define RIGHT_BCK_GPIO 73
#define RIGHT_FWD_GPIO 77

int fd;
int keepGoing = 1;
int gpsRet;

int gpioInit(){
	gpio_export(LEFT_FWD_GPIO);
	gpio_set_dir(LEFT_FWD_GPIO, 1);
	gpio_export(LEFT_BCK_GPIO);
	gpio_set_dir(LEFT_BCK_GPIO, 1);
	gpio_export(RIGHT_FWD_GPIO);
	gpio_set_dir(RIGHT_FWD_GPIO, 1);
	gpio_export(RIGHT_BCK_GPIO);
	gpio_set_dir(RIGHT_BCK_GPIO, 1);
}

int testTurn(){
	gpio_set_value(LEFT_FWD_GPIO, 1);
	gpio_set_value(RIGHT_FWD_GPIO,1);
	sleep(1);
	gpio_set_value(LEFT_FWD_GPIO, 0);
	gpio_set_value(RIGHT_FWD_GPIO,0);
	sleep(1);
	gpio_set_value(LEFT_BCK_GPIO, 1);
	gpio_set_value(RIGHT_FWD_GPIO,1);
	sleep(1);
	gpio_set_value(LEFT_BCK_GPIO, 0);
	gpio_set_value(RIGHT_FWD_GPIO,0);
	
}

float wrap180(float angle){
	if (angle > 180.0) angle -= 360.0;
	else if (angle < -180.0) angle += 360.0;
	return angle;
}

float getCompassHeading(void){
	return getHeading(fd);
}

int turn(float heading){
	float currentHeading = getHeading(fd);
	float deltaHeading = wrap180(heading - currentHeading); // deltaheading: negative means turn left, positive means turn right


	const int tolerance = 5;
	printf("deltaHeading: %f, currentHeading: %f, tolerance: %d\n", deltaHeading, currentHeading, tolerance);
	//while(abs((int) deltaHeading) > tolerance && keepGoing){
	while(keepGoing && abs(deltaHeading) > tolerance){
		if(deltaHeading > 0){ //turn right
			printf("**turning right**deltaHeading positive: %f, currentHeading: %f\n", deltaHeading, currentHeading);
			gpio_set_value(LEFT_FWD_GPIO, 1);
			gpio_set_value(LEFT_BCK_GPIO, 0);
			gpio_set_value(RIGHT_BCK_GPIO, 1);
			gpio_set_value(RIGHT_FWD_GPIO, 0); 		
		} else { // turn left
			printf("**turning left**deltaHeading negative: %f, currentHeading: %f\n", deltaHeading, currentHeading);
			gpio_set_value(LEFT_BCK_GPIO, 1);
			gpio_set_value(LEFT_FWD_GPIO, 0);
			gpio_set_value(RIGHT_FWD_GPIO, 1);
			gpio_set_value(RIGHT_BCK_GPIO, 0);			
		}
		currentHeading = getHeading(fd);
		deltaHeading = wrap180(heading - currentHeading);
	}

	gpio_set_value(LEFT_FWD_GPIO, 0);
	gpio_set_value(LEFT_BCK_GPIO, 0);
	gpio_set_value(RIGHT_FWD_GPIO, 0);
	gpio_set_value(RIGHT_BCK_GPIO, 0);
}

// 0.000011479429428388439 gps points per meter
short bbCheck(double m, double b, waypoint newWPCoord, double tolerance){ // tolerance in meters
	return newWPCoord.x + (newWPCoord.y + b)/((double) m) < tolerance * 0.000011479429428388439;
}

double findSlope(waypoint currentPos, waypoint newPos){
	return (newPos.y - currentPos.y)/(newPos.x - currentPos.x);
}

double * findLine(waypoint currentPos, waypoint newPos, double * mb){ //mb needs to have space for two doubles
	mb[0] = findSlope(currentPos, newPos);
	mb[1] = currentPos.y - mb[0]*currentPos.x;
	return mb;
}

int move(waypoint gpsCoord){
	int originalLineM, originalLineB;
}	

void signal_handler(int sig)
{
        printf( "Program Exited by User\n" );
	gpio_set_value(LEFT_FWD_GPIO, 0);
	gpio_set_value(LEFT_BCK_GPIO, 0);
	gpio_set_value(RIGHT_FWD_GPIO, 0);
	gpio_set_value(RIGHT_BCK_GPIO, 0);
	keepGoing = 0;
}

void moveForward(int useconds){
	gpio_set_value(LEFT_FWD_GPIO, 1);
        gpio_set_value(LEFT_BCK_GPIO, 0);
        gpio_set_value(RIGHT_FWD_GPIO, 1);
        gpio_set_value(RIGHT_BCK_GPIO, 0);
	usleep(useconds);
	gpio_set_value(LEFT_FWD_GPIO, 0);
        gpio_set_value(LEFT_BCK_GPIO, 0);
        gpio_set_value(RIGHT_FWD_GPIO, 0);
        gpio_set_value(RIGHT_BCK_GPIO, 0);
}

dataGPS getGPSlocation(void){
	return getGPS(gpsRet);
}

// 0.000011479429428388439 gps points per meter
void waypointManager(void){
	waypoint * newWP = (waypoint *) malloc(sizeof(waypoint));	
	dataGPS  curPosGPS;
	//int gpsRet;
	float turnHeading;
	double tolerance = 3; // tolerance in meters
	printf("Beginning\r\n");
	addWaypointxy(-87.322314, 39.483694);
	newWP = getCurrentWaypoint();
	printf("\r\nTrying to initialize GPS");
	//gpsRet = init_GPS();
	curPosGPS = getGPS(gpsRet);

	if(!curPosGPS.valid){
		for(int i = 0; i < 10 && !curPosGPS.valid; i++){
			sleep(5);
			printf("\r\nWaiting for valid GPS data....\r\n");
			curPosGPS = getGPS(gpsRet);
		}
	}
	waypoint * curPosWP = (waypoint *) malloc(sizeof(waypoint));
	curPosWP->x = curPosGPS.x;
	curPosWP->y = curPosGPS.y;
	turnHeading = angleBetweenPoints(*newWP, *curPosWP);
	turn(turnHeading);
	// at this point we should be pointing in the right direction

	double line_mb[2];
	findLine(*curPosWP, * newWP, line_mb);

	printf("\r\nCurrent position: x %lf, y %lf\r\n", curPosWP->x, curPosWP->y);
	printf("Waypoint to move to: x %lf, y %lf\r\n", newWP->x, newWP->y);
	printf("Turn heading: %f\r\n", turnHeading);
	printf("Line: m %lf, b %lf\r\n", line_mb[0], line_mb[1]);
	int waypointsLeft = 1;
	while(keepGoing && waypointsLeft){
		while(keepGoing && distanceBetweenPoints(*curPosWP, *newWP) > tolerance*0.000011479429428388439){
			// go straight		
			printf("Going straight\r\ndistance to target: %lf\r\ntolerance: %lf\r\n", distanceBetweenPoints(*curPosWP, *newWP), tolerance*0.000011479429428388439);
			gpio_set_value(LEFT_FWD_GPIO, 1);
			gpio_set_value(LEFT_BCK_GPIO, 0);
			gpio_set_value(RIGHT_FWD_GPIO, 1);
			gpio_set_value(RIGHT_BCK_GPIO, 0);	

			curPosGPS = getGPS(gpsRet);
			curPosWP->x = curPosGPS.x;
			curPosWP->y = curPosGPS.y;

			if(!bbCheck(line_mb[0], line_mb[1], * curPosWP, tolerance)){
				returnToPreviousWaypoint();
				printf("We're outside of the bounds. Returning to the previous waypoint\r\n");
				break; // out of the bounding box
			}
			sleep(2);
		}
		printf("We've hit the waypoint! Stopping the motors...\r\n");
		gpio_set_value(LEFT_FWD_GPIO, 0);
		gpio_set_value(LEFT_BCK_GPIO, 0);
		gpio_set_value(RIGHT_FWD_GPIO, 0);
		gpio_set_value(RIGHT_BCK_GPIO, 0);

		waypointsLeft = (advanceToNextWaypoint() != -1); // don't keep going if we've reached the end of the waypoints
		newWP = getCurrentWaypoint(); 	// if we've already advanced past the end this will give us the last WP, but 
						// we will break out of this loop anyway
		printf("KeepGoing: %d...should be 0 if we've hit the end of the waypoints", keepGoing);
	}
	free(curPosWP);
	free(newWP);
}

void initSensors(void){
	// TODO: init the GPS here
       fd = init_I2C();
       init_compass(fd);
       gpioInit();
	gpsRet = init_GPS();
}

void main(int * argv){
	signal(SIGINT, signal_handler);

	fd = init_I2C();
	init_compass(fd);
	gpioInit();
//	testTurn();
	/**turn(45.0);
	sleep(1);
	turn(280.0);
	sleep(1);
	turn(310.0);
	turn(180.0);
	**/
	waypointManager();
	//testTurn();
}


