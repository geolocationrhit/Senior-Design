typedef struct {
	float x;
	float y;
	float time;
	int valid;
} dataGPS;

int init_GPS(void);
dataGPS getGPS(int);
