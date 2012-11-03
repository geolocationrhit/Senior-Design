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

int init_GPS(void){
        int serial_file;
	FILE *config_location;
	if((config_location = fopen ("/sys/kernel/debug/omap_mux/uart1_txd","rb+")) == NULL)
		printf("Cannot set TX");
	char str[5] = {0};
	strcpy(str,"0");
	rewind(config_location);
	fwrite(&str,sizeof(char),strlen(str),config_location);
	fclose(config_location);
	if((config_location = fopen ("/sys/kernel/debug/omap_mux/uart1_rxd","rb+")) == NULL)
		printf("Cannot set RX");
	strcpy(str,"20");
	rewind(config_location);
	fwrite(&str,sizeof(char),strlen(str),config_location);
	fclose(config_location);
        serial_file = open( "/dev/ttyO1", O_RDWR);
	if(serial_file == -1)
		 fprintf(stderr, "Failed to open UART1: %m\n");
	else
        return serial_file;
}

int sendToGPS(int serial_file){
 	char buf[52] = {0};
        buf[0] = 'G';
        buf[1] = 0x74;
        buf[2] = 0b01100010;
	char buf_read[8] = {0};
	buf_read[0] = 0xB5;
	buf_read[1] = 0x62;
	buf_read[2] = 0x06;
	buf_read[3] = 0x3B;
	buf_read[4] = 0x00;
	buf_read[5] = 0x00;
	char ck_a = 0;
	char ck_b = 0;
	int i = 0;
	for(i=2; i < 6; i++){
		ck_a = ck_a + buf_read[i];
		ck_b = ck_b + ck_a;
	}
	buf_read[6] = ck_a;
	buf_read[7] = ck_b;
        if(write(serial_file,buf_read,8)!=8)
    			fprintf(stderr, "Failed to set to continous mode: %m\n");
	//ACK is 10B. SUCCESS = 2nd and 3rd bytes are 0x05 and 0x01 and a FAIL is 0x05 and 0x00. 
	char data[100] = {0};
	if(read(serial_file,data,62)!=62)
                fprintf(stderr,"Failed to read compass: %m\n");
	i = 0;
	for(i = 0; i < 62; i++)
		printf("%d: %x\n\r",i, data[i]);
	return 0;
}

dataGPS getGPS(int serial_file){
	dataGPS temp;
	temp.x = 0;
	temp.y = 0;
	temp.time = 0;
	temp.valid = 1;
	char latpos;
	char longpos;
	char isValid;
	
	int matches = -99;

	while(1){	
		char start[20] = {0};
		char data[100] = {0};
		if(!(read(serial_file,data,100)> 0))
		        fprintf(stderr,"Failed to read GPS: %m\n");
		if(!strncmp(data, "$GPGLL",6)) {
			if((matches = sscanf(data,"%6s,%lf,%c,%lf,%c,%lf,%c",start,&temp.y,&latpos,&temp.x,&longpos,&temp.time,&isValid)) == 0){
				printf("Yeah, that last thing was bogus");
			}
	break;
		} 
	}
	if(temp.x == 0)
		temp.valid = 0;
	else {
		if(latpos == 'S')
			temp.y = -temp.y;
		if(longpos == 'W')
			temp.x = -temp.x;
	}
	temp.x = (int)temp.x/100 + ((int)temp.x%100 + temp.x - (int)temp.x)/60.0;
	temp.y = (int)temp.y/100 + ((int)temp.y%100 + temp.y - (int)temp.y)/60.0;
	//temp.valid = 1;
	//temp.x = -87.322740;
	//temp.y = 39.483990;
	return temp;
}
