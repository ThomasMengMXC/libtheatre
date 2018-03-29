CFLAGS = -Wall -pedantic -g -std=gnu11 -O3
LFLAGS = -ltinfo -lncurses
CC = gcc

all: libtheatre.so

install: all 
	mkdir -p /usr/local/include/theatre
	cp *.h /usr/local/include/theatre
	cp libtheatre.so /usr/local/lib

libtheatre.so: play.o stage.o scene.o props.o screen.o layer.o sprite.o colour.o
	$(CC) -shared *.o $(LFLAGS) -o libtheatre.so

play.o: play.c play.h
	$(CC) $(CFLAGS) -fPIC -c play.c

stage.o: stage.c stage.h stage_struct.h
	$(CC) $(CFLAGS) -fPIC -c stage.c

scene.o: scene.c scene.h scene_struct.h
	$(CC) $(CFLAGS) -fPIC -c scene.c

props.o: props.c props.h props_struct.h
	$(CC) $(CFLAGS) -fPIC -c props.c

screen.o: screen.c screen.h sprite.h screen_struct.h
	$(CC) $(CFLAGS) -fPIC -c screen.c

layer.o: layer.c layer.h sprite.h layer_struct.h
	$(CC) $(CFLAGS) -fPIC -c layer.c

sprite.o: sprite.c sprite.h sprite_struct.h
	$(CC) $(CFLAGS) -fPIC -c sprite.c

colour.o: colour.c colour.h
	$(CC) $(CFLAGS) -fPIC -c colour.c

clean:
	rm -f *.o
	rm -f libtheatre.so

uninstall: /usr/local/lib/libtheatre.so
	rm -f /usr/local/lib/libtheatre.so
	rm -f /usr/local/include/theatre/*
	rm -rf /usr/local/include/theatre
