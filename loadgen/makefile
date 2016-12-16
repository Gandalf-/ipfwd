# loadgen.c 
# author: Austin Voecks
# 
# Program that simluates CPU load by doing some math and then waiting for an
# empirically determined number of nanoseconds. 
# 

CC=cc
FLAGS=
SRC=loadgen.c
OUT=~/ipfwd/bin/loadgen

all: ${SRC}
	${CC} ${FLAGS} -o ${OUT} ${SRC}

clean:
	@rm -rf ${OUT} *.o *.core
	@echo done
