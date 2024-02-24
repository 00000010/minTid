CC = gcc
CFLAGS = -Wall -pedantic -lncurses

progName := minTid

all: $(progName)

$(progName): *.o
	$(CC) $^ $(CFLAGS) -o $@

%.o: %.c
	$(CC) -c $^

clean:
	echo $(progName) 
