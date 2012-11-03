

CC:=/usr/bin/gcc
CFLAGS := -g -std=c99

.PHONY: all
all: movement sharedLibs

.PHONY: clean
clean: cleanAll

movement.o: movement.c
	gcc $(CFLAGS) -c $^ -o $@

GPSLib/gps.o: GPSLib/gps.c
	gcc $(CFLAGS) -c $^ -lm -o $@

Compass/compass.o: Compass/compass.c
	gcc $(CFLAGS) -c $^ -o $@

gpio.o: gpio.c
	gcc $(CFLAGS) -c $^ -o $@

Waypoints/waypoint.o: Waypoints/waypoint.c
	gcc $(CFLAGS) -c $^ -o $@ 

movement: movement.o gpio.o GPSLib/gps.o Compass/compass.o Waypoints/waypoint.o
	gcc $(CFLAGS) $^ -lm -o $@

.PHONY: sharedLibs
sharedLibs: Compass/compass.c GPSLib/gps.c
	gcc -shared -fPIC -o sharedLibs/compassLib.so Compass/compass.c
	gcc -shared -fPIC -o sharedLibs/gpsLib.so GPSLib/gps.c
	gcc -shared -fPIC -o sharedLibs/waypointLib.so Waypoints/waypoint.c
	gcc $(CFLAGS) -shared -fPIC -o sharedLibs/movementLib.so movement.c Waypoints/waypoint.c Compass/compass.c GPSLib/gps.c gpio.c

cleanAll: 
	rm movement movement.o gpio.o GPSLib/gps.o Compass/compass.o sharedLibs/*


