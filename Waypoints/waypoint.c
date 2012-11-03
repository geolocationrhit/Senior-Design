#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <string.h>
#include <math.h>
#include "waypoint.h"

#define pi 3.14159265359
#define d2r 0.0174532925199433 

waypoint waypointList[100] = {0};
int numberOfWaypoints = 0;
int currentWaypoint = 0;

int addWaypointxy(double x, double y){
	waypoint newwp;
	newwp.x = x;
	newwp.y = y;
	return addWaypoint(newwp);
}

int addWaypoint(waypoint newWaypoint){
	if(numberOfWaypoints < 100){
		waypointList[numberOfWaypoints] = newWaypoint;
		numberOfWaypoints++;
		return 0;	
	}
	else
		return -1;
}
waypoint * getCurrentWaypoint(void){
	if(numberOfWaypoints <= 0)
		return NULL;
	return &waypointList[currentWaypoint];
}

int getNumberOfWaypoints(void){
	return numberOfWaypoints;
}

int advanceToNextWaypoint(void){
	if(currentWaypoint >= numberOfWaypoints)
		return -1;
	else
		return ++currentWaypoint;
}

int returnToPreviousWaypoint(void){
	if(currentWaypoint == 0)
		return -1;
	else
		return --currentWaypoint;
}

double distanceBetweenPoints(waypoint A, waypoint B){
	/** double deltax = (B.x - A.x) * d2r;
	double deltay = (B.y - A.y) * d2r;

	double c = 2 * atan2(sqrt(a), sqrt(1-a));
	double d = 6367 * c * 1000;
	**/
	double d = sqrt(pow(B.x - A.x,2)+pow(B.y - A.y,2));
	return d;
}

float angleBetweenPoints(waypoint A, waypoint B){
	float deltax = B.x - A.x;
	float deltay = B.y - A.y;
	//float angle = atan(deltay/deltax);
	float angle = atan2(deltax,deltay) * 180 / pi;
	if (angle < 0)
		angle = angle + 360;
        return angle;
}
