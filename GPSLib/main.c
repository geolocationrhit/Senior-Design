#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <string.h>
#include <math.h>
#include "gps.h"

int main(int argc, char *argv){
int serial_file = init_GPS();
//sendToGPS(serial_file);
dataGPS fake = getGPS(serial_file);
if(fake.valid == 0)
	printf("Invalid GPS Point\n\r");
else
	printf("X: %2.10lf Y: %2.10lf Time: %lf\n\r",fake.x, fake.y, fake.time);
}
