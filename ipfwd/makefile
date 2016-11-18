FLAGS= -g -lm
DEBUG= -D DEBUG
DEPS= ipfwd.o ipfwd.h

ipfwd: ${DEPS}
	cc ${FLAGS} -o ipfwd ipfwd.c

debug: ${DEPS}
	cc ${DEBUG} ${FLAGS} -o ipfwd ipfwd.c 

clean:
	rm -rf ipfwd *.o
