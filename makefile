all: minTid

minTid: minTid.c
	cc minTid.c -pedantic -Wall -lncurses -o minTid
