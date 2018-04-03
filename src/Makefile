CFLAGS = -Wall -pedantic -g -std=gnu11 -fPIC -O3
LFLAGS = -ltinfo -lncurses
CC = gcc

all: libtheatre.so

install: all 
	mkdir -p /usr/include/theatre
	cp *.h /usr/include/theatre
	cp libtheatre.so /usr/lib

libtheatre.so: play.o stage.o scene.o props.o screen.o layer.o sprite.o colour.o
	$(CC) -shared *.o $(LFLAGS) -o libtheatre.so

play.o: play.c play.h
	$(CC) $(CFLAGS) -c play.c

stage.o: stage.c stage.h stage_struct.h
	$(CC) $(CFLAGS) -c stage.c

scene.o: scene.c scene.h scene_struct.h
	$(CC) $(CFLAGS) -c scene.c

props.o: props.c props.h props_struct.h
	$(CC) $(CFLAGS) -c props.c

screen.o: screen.c screen.h sprite.h screen_struct.h
	$(CC) $(CFLAGS) -c screen.c

layer.o: layer.c layer.h sprite.h layer_struct.h
	$(CC) $(CFLAGS) -c layer.c

sprite.o: sprite.c sprite.h sprite_struct.h
	$(CC) $(CFLAGS) -c sprite.c

colour.o: colour.c colour.h
	$(CC) $(CFLAGS) -c colour.c

clean:
	rm -f *.o
	rm -f libtheatre.so

uninstall: /usr/lib/libtheatre.so
	rm -f /usr/lib/libtheatre.so
	rm -f /usr/include/theatre/*
	rm -rf /usr/include/theatre
