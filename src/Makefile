CFLAGS = -Wall -pedantic -g -std=gnu11
LFLAGS = -Wall -ltheatre -ltinfo -lncursesw
CC = gcc

all: libtheatre.so

install: all 
	mkdir -p /usr/local/include/theatre
	cp *.h /usr/local/include/theatre
	cp libtheatre.so /usr/local/lib

libtheatre.so: play.o stage.o scene.o screen.o layer.o sprite.o colour.o theatre.h
	$(CC) -shared -o libtheatre.so *.o -lm

play.o: play.c play.h
	$(CC) $(CFLAGS) -fPIC -c play.c

stage.o: stage.c stage.h
	$(CC) $(CFLAGS) -fPIC -c stage.c

scene.o: scene.c scene.h
	$(CC) $(CFLAGS) -fPIC -c scene.c

screen.o: screen.c screen.h sprite.h
	$(CC) $(CFLAGS) -fPIC -c screen.c

layer.o: layer.c layer.h sprite.h
	$(CC) $(CFLAGS) -fPIC -c layer.c

sprite.o: sprite.c sprite.h
	$(CC) $(CFLAGS) -fPIC -c sprite.c

colour.o: colour.c
	$(CC) $(CFLAGS) -fPIC -c colour.c

clean:
	rm -f *.o
	rm -f libtheatre.so

uninstall: /usr/local/lib/libtheatre.so
	rm -f /usr/local/lib/libtheatre.so
	rm -f /usr/local/include/theatre/*
	rm -rf /usr/local/include/theatre
