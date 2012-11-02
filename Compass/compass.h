

int init_I2C(void);
void init_compass(int fd);
float getHeading(int fd);
void calibrate_compass(int fd);
