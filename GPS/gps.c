#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include "gps.h"

#define pi 3.14159265359
#define d2r 0.0174532925199433 

gpsPoint convertGPSToPoint(coordinate location){
	float latdegrees = location.latdegrees + (location.latminutes/60) + (location.latseconds/3600);
	float longdegrees = location.longdegrees + (location.longminutes/60) + (location.longseconds/3600);
	if(location.latcardinal == 'S')
		latdegrees = -latdegrees;
	if(location.longcardinal == 'W')
		longdegrees = -longdegrees;	
	gpsPoint result;
	result.x = longdegrees;
	result.y = latdegrees;
	return result;
}

float distanceBetweenPoints(gpsPoint A, gpsPoint B){
	float deltax = (B.x - A.x) * d2r;
	float deltay = (B.y - A.y) * d2r;
	float a = pow(sin(deltay/2.0),2) + cos(A.y*d2r)*cos(B.y*d2r)*pow(sin(deltax/2.0),2);
	float c = 2 * atan2(sqrt(a), sqrt(1-a));
	float d = 6367 * c * 1000;
	return d;
}

float angleBetweenPoints(gpsPoint A, gpsPoint B){
	float deltax = B.x - A.x;
	float deltay = B.y - A.y;
	//float angle = atan(deltay/deltax);
	float angle = atan2(deltax,deltay) * 180 / pi + 180;
	if(angle > 0){
	if(deltay > 0)
		return angle;
	else
		return 180 + angle;	
	}
	else {
	if (deltax > 0)
		return 180 + angle;
	else 
		return 360 + angle;
	}
}
