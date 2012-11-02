#include "i2c-dev.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include "compass.h"

int main(int argc, char *argv){
        int fd = init_I2C();
        init_compass(fd);
        while(1){
                usleep(200000);
                float heading = getHeading(fd);
                printf("Heading:%3.1f\n",heading);
        }
}
