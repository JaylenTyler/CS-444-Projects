\CC = gcc
CFLAGS = -Wall -Wextra -Werror

OBJS = image.o block.o free.o inode.o pack.o dir.o ls.o

all: testfs

test: testfs
	./testfs

testfs: $(OBJS) testfs.o
	$(CC) $(CFLAGS) -o testfs $(OBJS) testfs.o

image.o: image.c image.h
	$(CC) $(CFLAGS) -c image.c

block.o: block.c block.h image.h free.h
	$(CC) $(CFLAGS) -c block.c

free.o: free.c free.h
	$(CC) $(CFLAGS) -c free.c

inode.o: inode.c inode.h block.h free.h
	$(CC) $(CFLAGS) -c inode.c

pack.o: pack.c pack.h
	$(CC) $(CFLAGS) -c pack.c

dir.o: dir.c dir.h inode.h block.h pack.h
	$(CC) $(CFLAGS) -c dir.c

ls.o: ls.c dir.h
	$(CC) $(CFLAGS) -c ls.c

testfs.o: testfs.c image.h block.h inode.h free.h dir.h ctest.h
	$(CC) $(CFLAGS) -DCTEST_ENABLE -c testfs.c

clean:
	rm -f *.o testfs libvvsfs.a



