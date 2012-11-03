typedef struct {
	double x;
	double y;
	double time;
	int valid;
} dataGPS;

int init_GPS(void);
dataGPS getGPS(int);
