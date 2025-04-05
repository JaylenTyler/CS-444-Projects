# Makefile for hellothread project

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
TARGET = hellothread

all: $(TARGET)

$(TARGET): hellothread.c
	$(CC) $(CFLAGS) -o $(TARGET) hellothread.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
