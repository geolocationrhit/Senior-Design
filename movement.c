/*	movement.c *
*	This is going to be written without any bugs 
*/

#include "GPS/gps.h"

#define LEFT_FWD_GPIO 71
#define LEFT_BCK_GPIO 73
#define RIGHT_FWD_GPIO 75
#define RIGHT_BCK_GPIO 77

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

int turn(float heading){
	gpio_set_value(LEFT_FWD_GPIO, 1);
	gpio_set_value(RIGHT_BCK_GPIO,1);
	sleep(1);
	gpio_set_value(LEFT_FWD_GPIO, 0);
	gpio_set_value(RIGHT_BCK_GPIO,0);
}

short bbCheck(float m, float b, gpsPoint newGPSCoord, float tolerance){
	return newGPSCoord.x + (newGPSCoord.y + b)/((float) m) < tolerance;
}

int move(gpsPoint gpsCoord){
	int originalLineM, originalLineB;
}	

void main(int * argv){
	;
}


