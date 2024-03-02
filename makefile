CC = gcc
CFLAGS = -Wall -pedantic -lncurses

progName := minTid

all: $(progName)

$(progName): *.o
	$(CC) $^ $(CFLAGS) -o ./build/$@

%.o: %.c
	$(CC) -c $^

clean:
	rm *.o
	cd ./build && rm minTid
