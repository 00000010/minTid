all: minTid

minTid: minTid.c
	cc minTid.c -ansi -pedantic -Wall -lncurses -o minTid
