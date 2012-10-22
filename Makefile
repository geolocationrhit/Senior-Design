

CC:=/usr/bin/gcc
CFLAGS := -g -std=c99

.PHONY: all
all: movement

.PHONY: clean
clean: cleanAll

movement.o: movement.c
	gcc $(CFLAGS) -c $^ -o $@

GPS/gps.o: GPS/gps.c
	gcc $(CFLAGS) -c $^ -lm -o $@

gpio.o: gpio.c
	gcc $(CFLAGS) -c $^ -o $@

movement: movement.o gpio.o GPS/gps.o
	gcc $(CFLAGS) $^ -lm -o $@

cleanAll: 
	rm movement movement.o gpio.o GPS/gps.o


