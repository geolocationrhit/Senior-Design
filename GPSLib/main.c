#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <string.h>
#include <math.h>

typedef struct {
	float x;
	float y;
	float time;
	int valid;
} dataGPS;

int init_GPS(void){
        int serial_file;
	FILE *config_location;
	if((config_location = fopen ("/sys/kernel/debug/omap_mux/uart1_txd","ab")) == NULL)
		printf("Cannot set TX");
	fprintf(config_location,"%d\n",0);
	if((config_location = fopen ("/sys/kernel/debug/omap_mux/uart1_rxd","ab")) == NULL)
		printf("Cannot set RX");
	fprintf(config_location,"%d\n",20);
        serial_file = open( "/dev/ttyO1", O_RDWR);
	if(serial_file == -1)
		 fprintf(stderr, "Failed to open UART1: %m\n");
	else
        return serial_file;
}

dataGPS getGPS(int serial_file){
	printf("Success\n\r");
	dataGPS fake;
	fake.x = 0;
	fake.y = 0;
	fake.time = 0;
	fake.valid = 1;
	char latpos;
	char longpos;
	char isValid;
	char start[20] = {0};
	int matches = -99;

	while(1){	
		char data[100] = {0};
		if(!(read(serial_file,data,100)> 0))
		        fprintf(stderr,"Failed to read GPS: %m\n");
		//printf("Orig: %s, first char: %c ----- \n", data, data[0]);
		if(!strncmp(data, "$GPGLL",6)) {
			printf("Orig matched: %s", data);
			if((matches = sscanf(data,"%6s,%f,%c,%f,%c,%f,%c",start,&fake.y,&latpos,&fake.x,&longpos,&fake.time,&isValid)) == 0){
				printf("Yeah, that last thing was bogus");
			} else {
				printf("crap y:%f pos:%c x:%f pos:%c time:%f valid:%c\n\r",
						fake.y,latpos,fake.x,longpos,fake.time,isValid);
					//printf("%c\n\r",valid);
				printf("\nMatches: %d", matches);
			}
	break;
		} 
	}
	return fake;
}



int main(int argc, char *argv){
int serial_file = init_GPS();
dataGPS fake = getGPS(serial_file);
}
