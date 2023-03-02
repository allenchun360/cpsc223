CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g3

Chomp: chomp_main.o chomp.o string_helpers.o gmap.o
	${CC} -o $@ ${CFLAGS} $^

chomp_main.o: chomp.h string_helpers.h gmap.h
gmap.o: gmap.h
string_helpers.o: string_helpers.h
chomp.o: chomp.h
