CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g3

all: BlottoGA Unit

BlottoGA: blotto_ga.o strategy.o population.o blotto.o arrays.o ga_helpers.o
	${CC} ${CCFLAGS} -o $@ $^ -lm

Unit: ga_unit.o strategy.o population.o blotto.o arrays.o ga_helpers.o
	${CC} ${CCFLAGS} -o $@ $^ -lm

blotto_ga.o: strategy.h population.h ga_helpers.h
strategy.o: strategy.h arrays.h blotto.h
population.o: population.h strategy.h
ga_unit.o: strategy.h population.h ga_helpers.h
blotto.o: blotto.h
ga_helpers.o: ga_helpers.h strategy.h population.h
arrays.o: arrays.h