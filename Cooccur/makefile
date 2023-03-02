CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g3

all: Cooccur GmapUnit CooccurUnit

Cooccur: cooccur_main.o cooccur.o gmap.o string_key.o
	${CC} ${CCFLAGS} -o $@ $^ -lm

GmapUnit: gmap_unit.o gmap.o gmap_test_functions.o string_key.o gmap.o
	${CC} ${CCFLAGS} -o $@ $^ -lm

CooccurUnit: cooccur_unit.o gmap_test_functions.o cooccur.o gmap.o string_key.o
	${CC} ${CCFLAGS} -o $@ $^ -lm

cooccur_main.o: cooccur.h
gmap.o: gmap.h
gmap_test_functions.o: gmap.h gmap_test_functions.h 
string_key.o: string_key.h
cooccur.o: cooccur.h gmap.h string_key.h
cooccur_unit.o: gmap_test_functions.h cooccur.h
gmap_unit.o: gmap.h gmap_test_functions.h string_key.h