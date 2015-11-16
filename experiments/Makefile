CC=g++
CFLAGS=-Wall -Werror

all: experiment
experiment: main.o
	$(CC) $(CFLAGS) -o experiment main.o -lpthread
main.o: main.cc
	$(CC) $(CFLAGS) -c main.cc
.PHONY:
clean:
	-rm -f experiment *.o *~
