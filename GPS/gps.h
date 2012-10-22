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

gpsPoint convertGPSToPoint(coordinate location);
float distanceBetweenPoints(gpsPoint A, gpsPoint B);
float angleBetweenPoints(gpsPoint A, gpsPoint B);
