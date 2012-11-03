/*	movement.c *
*	This is going to be written without any bugs 
*/

#include "GPS/gps.h"
#include "gpio.h"
#include <unistd.h>
#include "i2c-dev.h"
#include "Compass/compass.h" // booooooo
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define LEFT_BCK_GPIO 71
#define LEFT_FWD_GPIO 73
#define RIGHT_BCK_GPIO 75
#define RIGHT_FWD_GPIO 77

int fd;
int keepGoing = 1;

enum motorOps {
	forward,
	right,
	left
};

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

int motorControl(enum motorOps direction, short active){
	switch(direction){
		forward: break;
		right: break;
		left: break;
	}
}

int testTurn(){
	gpio_set_value(LEFT_FWD_GPIO, 1);
	gpio_set_value(RIGHT_BCK_GPIO,1);
	sleep(1);
	gpio_set_value(LEFT_FWD_GPIO, 0);
	gpio_set_value(RIGHT_BCK_GPIO,0);

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
/*	while(!((deltaHeading < tolerance) || (deltaHeading - 360 > -tolerance)) && keepGoing){
		//printf("In the if! We should be turning....\n");
		if(deltaHeading > 0){
			printf("deltaHeading positive: %f, currentHeading: %f\n", deltaHeading, currentHeading);
			if(deltaHeading < 180){ //turn left
				gpio_set_value(LEFT_BCK_GPIO, 1);
				gpio_set_value(LEFT_FWD_GPIO, 0);
				gpio_set_value(RIGHT_FWD_GPIO, 1);
				gpio_set_value(RIGHT_BCK_GPIO, 0);
			} else { //turn right
				gpio_set_value(LEFT_FWD_GPIO, 1);
				gpio_set_value(LEFT_BCK_GPIO, 0);
				gpio_set_value(RIGHT_BCK_GPIO, 1);
				gpio_set_value(RIGHT_FWD_GPIO, 0);
			}
		} else {
			printf("deltaHeading negative: %f, currentHeading: %f\n", deltaHeading, currentHeading);
			if(deltaHeading < 180){ //turn right
				gpio_set_value(LEFT_FWD_GPIO, 1);
				gpio_set_value(LEFT_BCK_GPIO, 0);
				gpio_set_value(RIGHT_BCK_GPIO, 1);
				gpio_set_value(RIGHT_FWD_GPIO, 0); 
			} else { //turn left
				gpio_set_value(LEFT_BCK_GPIO, 1);
				gpio_set_value(LEFT_FWD_GPIO, 0);
				gpio_set_value(RIGHT_FWD_GPIO, 1);
				gpio_set_value(RIGHT_BCK_GPIO, 0);
			}
		} 
		//usleep(100);
		currentHeading = getHeading(fd);
		deltaHeading = currentHeading - heading;
	}
*/
	gpio_set_value(LEFT_FWD_GPIO, 0);
	gpio_set_value(LEFT_BCK_GPIO, 0);
	gpio_set_value(RIGHT_FWD_GPIO, 0);
	gpio_set_value(RIGHT_BCK_GPIO, 0);
}

// 0.000011479429428388439 gps points per meter
short bbCheck(double m, double b, gpsPoint newGPSCoord, double tolerance){ // tolerance in meters
	return newGPSCoord.x + (newGPSCoord.y + b)/((double) m) < tolerance * 0.000011479429428388439;
}

double findSlope(gpsPoint currentPos, gpsPoint newPos){
	return (newPos.y - currentPos.y)/(newPos.x - currentPos.x);
}

double * findIntercept(gpsPoint currentPos, gpsPoint newPos, double * mb){ //mb needs to have space for two doubles
	mb[0] = findSlope(currentPos, newPos);
	mb[1] = currentPos.y - mb[0]*currentPos.x;
	return mb;
}

int move(gpsPoint gpsCoord){
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

void main(int * argv){
	signal(SIGINT, signal_handler);

	fd = init_I2C();
	init_compass(fd);
	gpioInit();
	//testTurn();
	turn(45.0);
	sleep(1);
	turn(280.0);
	sleep(1);
	turn(310.0);
	turn(180.0);
	//testTurn();
}


