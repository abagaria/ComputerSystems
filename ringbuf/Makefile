CC = gcc
CFLAGS = -Wall

all: ringbuf

ringbuf: ringbuf.o
	$(CC) $(CFLAGS) -lpthread -o ringbuf ringbuf.o

ringbuf.o: ringbuf.h ringbuf.c
	$(CC) $(CFLAGS) -c ringbuf.c

clean:
	rm ringbuf.o
	rm ringbuf