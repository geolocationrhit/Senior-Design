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
        calibrate_compass(fd);
}
