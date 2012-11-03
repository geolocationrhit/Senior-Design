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



int main(int argc, char *argv){
	waypoint w1;
	w1.x = 0;
	w1.y = 0;
	//addWaypointxy(1.0,2.0);
	//waypoint * returned = getCurrentWaypoint();
	//printf("x: %f y: %f\r\n",returned->x, returned->y);
	waypoint w2;
	w2.x = 2;
	w2.y = 1;
	/*addWaypoint(w2);
	waypoint * returned2 = getCurrentWaypoint();
	printf("x: %f y: %f\r\n",returned2->x, returned2->y);
	waypoint * returned3 = getCurrentWaypoint();
	printf("x: %f y: %f\r\n",returned3->x, returned3->y);
	*/
	float angle = angleBetweenPoints(w1,w2);
	printf("\n\rAngle: %f\n\r",angle);

	
}
