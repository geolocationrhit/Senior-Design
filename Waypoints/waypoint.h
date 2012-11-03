typedef struct {
	double x;
	double y;
} waypoint;

int addWaypoint(waypoint);
waypoint * getCurrentWaypoint(void);
int addWaypointxy(double, double);
int getNumberOfWaypoints(void);
double distanceBetweenPoints(waypoint, waypoint);
float angleBetweenPoints(waypoint, waypoint);
int advanceToNextWaypoint(void);
int returnToPreviousWaypoint(void);
