#include "i2c-dev.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include "compass.h"
//This is the address of the Compass on the I2C Bus
#define ADDRESS 0x22

int init_I2C(void){
        int fd;
        fd = open( "/dev/i2c-3", O_RDWR);
        if( ioctl(fd, I2C_SLAVE, ADDRESS) < 0)
        {
                fprintf(stderr, "Failed to set slave address: %m\n");
                return 2;
        }
        return fd;
}

//Setup the compass and set it to continous output mode
void init_compass(int fd){
        char buf[10] = {0};
        buf[0] = 'G';
        buf[1] = 0x74;
        buf[2] = 0b01100010;
        if(write(fd,buf,3)!=3)
    			fprintf(stderr, "Failed to set to continous mode: %m\n");
}

void calibrate_compass(int fd){
	char buf[10] = {0};
	buf[0] = 'C';
	if(write(fd,buf,1)!=1)
			fprintf(stderr, "Failed to start calibration: %m\n");
	printf("Calibration started\n\r");
	sleep(10);
	buf[0] = 'E';
	if(write(fd,buf,1)!=1)
			fprintf(stderr, "Failed to exit calibration: %m\n");
	printf("Calibration complete\n\r");
}

float getHeading(int fd){
        char data[10] = {0};
        if(read(fd,data,2)!=2)
                fprintf(stderr,"Failed to read compass: %m\n");
        float result = ((data[0] << 8) + data[1])/10.0;
        return result;
}
