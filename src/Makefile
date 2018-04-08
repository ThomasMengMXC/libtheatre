CFLAGS = -Wall -pedantic -g -std=gnu11 -fPIC -O3
LFLAGS = -ltinfo -lncurses
DIRECTORY = $(HOME)
CC = gcc

all: libtheatre.so

install: all 
	mkdir -p $(DIRECTORY)/include/theatre $(DIRECTORY)/lib
	cp *.h $(DIRECTORY)/include/theatre
	cp libtheatre.so $(DIRECTORY)/lib

libtheatre.so: play.o stage.o scene.o props.o screen.o layer.o sprite.o vector2D.o colour.o
	$(CC) -shared *.o $(LFLAGS) -o libtheatre.so

play.o: play.c play.h
	$(CC) $(CFLAGS) -c play.c

stage.o: stage.c stage.h
	$(CC) $(CFLAGS) -c stage.c

scene.o: scene.c scene.h
	$(CC) $(CFLAGS) -c scene.c

props.o: props.c props.h
	$(CC) $(CFLAGS) -c props.c

screen.o: screen.c screen.h sprite.h
	$(CC) $(CFLAGS) -c screen.c

layer.o: layer.c layer.h sprite.h
	$(CC) $(CFLAGS) -c layer.c

sprite.o: sprite.c sprite.h
	$(CC) $(CFLAGS) -c sprite.c

vector2D.o: vector2D.c vector2D.h
	$(CC) $(CFLAGS) -c vector2D.c

colour.o: colour.c colour.h
	$(CC) $(CFLAGS) -c colour.c

clean:
	rm -f *.o
	rm -f libtheatre.so

uninstall:
	rm -f $(DIRECTORY)/lib/libtheatre.so
	rm -f $(DIRECTORY)/include/theatre/*
	rm -rf $(DIRECTORY)/include/theatre
