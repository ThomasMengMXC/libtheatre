DIRECTORY = $(HOME)
CFLAGS = -Wall -pedantic -std=c89 -fPIC -O3
LFLAGS = -lncursesw
CC = gcc
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

all: libtheatre.so

install: all 
	mkdir -p $(DIRECTORY)/include/theatre $(DIRECTORY)/lib
	cp *.h $(DIRECTORY)/include/theatre
	cp libtheatre.so $(DIRECTORY)/lib

analyse:
	clang-tidy -fix *.c -- -std=c11

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
