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
}

int turn(float heading){
	float currentHeading = getHeading(fd);
	float deltaHeading = currentHeading - heading;

	const int tolerance = 1;
	printf("deltaHeading: %f, tolerance: %d\n", deltaHeading, tolerance);
	while(abs(currentHeading - heading) > tolerance && currentHeading heading - 360
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

short bbCheck(float m, float b, gpsPoint newGPSCoord, float tolerance){
	return newGPSCoord.x + (newGPSCoord.y + b)/((float) m) < tolerance;
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
	//testTurn();
}


