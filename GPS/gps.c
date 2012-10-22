#include <math.h>
#define pi 3.14159265359
#define d2r 0.0174532925199433
typedef struct {
        float x;
        float y;
} gpsPoint;

typedef struct {
        float latdegrees;
        float latminutes;
        float latseconds;
        float longdegrees;
        float longminutes;
        float longseconds;
        char latcardinal;
        char longcardinal;
} coordinate;

point convertGPSToPoint(coordinate location){
        float latdegrees = location.latdegrees + (location.latminutes/60) + (location.latseconds/3600);
        float longdegrees = location.longdegrees + (location.longminutes/60) + (location.longminutes/3600);
        if(location.latcardinal == 'S')
                latdegrees = -latdegrees;
        if(location.longcardinal == 'W')
                longdegrees = -longdegrees;
        point result;
        result.x = longdegrees;
        result.y = latdegrees;
        return result;
}

float distanceBetweenPoints(point A, point B){
        float deltax = (B.x - A.x) * d2r;
        float deltay = (B.y - A.y) * d2r;
        float a = pow(sin(deltay/2.0),2) + cos(A.y*d2r)*cos(B.y*d2r)*pow(sin(deltax/2.0),2);
        float c = 2 * atan2(sqrt(a), sqrt(1-a));
        float d = 6367 * c * 1000;
        return d;
}

//This needs to be fixed. I thought I had the conversion from +/- 90 to 360 working, but it isn't.
float angleBetweenPoints(point A, point B){
        float deltax = B.x - A.x;
        float deltay = B.y - A.y;
        float angle = atan(deltay/deltax);
        angle = angle*180/pi;
        if(deltax > 0 && deltay > 0)
                angle = 90 - angle;
        if(deltax > 0 && deltay < 0)
                angle = 90 - angle;
        return angle;
}