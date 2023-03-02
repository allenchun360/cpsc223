CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g3

TSP: location.o tsp.o cities.o
	${CC} ${CCFLAGS} -o $@ $^ -lm

Unit: location.o cities.o cities_unit.o
	${CC} ${CCFLAGS} -o $@ $^ -lm

tsp.o: location.h cities.h
location.o: location.h
cities.o: location.h cities.h
cities_unit.o: location.h cities.h
