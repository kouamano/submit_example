CC=pgcc -Minfo=accel -ta=tesla
MODULE=/usr/bin/modulecmd bash
 
all: acctest.out sample.out

acctest.out: acctest.c
	eval `${MODULE} purge`;eval `${MODULE} load pgi/18.10`; ${CC} $^ -o $@;eval `${MODULE} purge`;

sample.out: sample.c
	eval `${MODULE} purge`;eval `${MODULE} load pgi/18.10`; ${CC} $^ -o $@;eval `${MODULE} purge`;

clean: 
	-@rm *.out acctest.sh sample.sh
