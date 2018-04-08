CFLAGS = -Wall -pedantic -g -std=gnu11 -fPIC -O3
LFLAGS = -ltinfo -lncurses
DIRECTORY = $(HOME)
CC = gcc
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

all: libtheatre.so

install: all 
	mkdir -p $(DIRECTORY)/include/theatre $(DIRECTORY)/lib
	cp *.h $(DIRECTORY)/include/theatre
	cp libtheatre.so $(DIRECTORY)/lib

libtheatre.so: $(OBJECTS)
	$(CC) -shared *.o $(LFLAGS) -o libtheatre.so

%.o : %.c %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	rm -f libtheatre.so

uninstall:
	rm -f $(DIRECTORY)/lib/libtheatre.so
	rm -f $(DIRECTORY)/include/theatre/*
	rm -rf $(DIRECTORY)/include/theatre
