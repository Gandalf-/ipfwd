# makefile
# author: Austin Voecks
# 
# Monitors system load using uptime() to set probability of IPFW's first rule
# allowing any packet. This rule increases performance by sacrificing complete
# security. The probability is dynamic based on the current load.
# 

FLAGS= -g -lm
DEPS= ipfwd.o ipfwd.h

ipfwd: ${DEPS}
	cc ${FLAGS} -o ipfwd ipfwd.c

debug: ${DEPS}
	cc -D DEBUG ${FLAGS} -o ipfwd ipfwd.c 

clean:
	rm -rf ipfwd *.o
