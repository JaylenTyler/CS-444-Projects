CC = gcc
CFLAGS = -Wall -Wextra -Werror

OBJS = image.o block.o

all: testfs

test: testfs
	./testfs

testfs: $(OBJS) testfs.o
	$(CC) $(CFLAGS) -o testfs $(OBJS) testfs.o

image.o: image.c image.h
	$(CC) $(CFLAGS) -c image.c

block.o: block.c block.h image.h
	$(CC) $(CFLAGS) -c block.c

testfs.o: testfs.c image.h block.h ctest.h
	$(CC) $(CFLAGS) -DCTEST_ENABLE -c testfs.c

clean:
	rm -f *.o testfs libvvsfs.a


