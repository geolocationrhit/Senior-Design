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

waypoint waypointList[100] = {0};
int numberOfWaypoints = 0;

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
	return &waypointList[numberOfWaypoints - 1];
}

int getNumberOfWaypoints(void){
	return numberOfWaypoints;
}
