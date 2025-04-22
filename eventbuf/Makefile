CC=gcc
CFLAGS=-Wall -Wextra -Werror

OBJS=pc.o eventbuf.o

all: pc

pc: $(OBJS)
	$(CC) $(CFLAGS) -o pc $(OBJS) -lpthread -lrt

pc.o: pc.c eventbuf.h
	$(CC) $(CFLAGS) -c pc.c

eventbuf.o: eventbuf.c eventbuf.h
	$(CC) $(CFLAGS) -c eventbuf.c

clean:
	rm -f *.o pc
