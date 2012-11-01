typedef struct {
	double x;
	double y;
} waypoint;

int addWaypoint(waypoint);
waypoint * getCurrentWaypoint(void);
int addWaypointxy(double, double);
int getNumberOfWaypoints(void);
