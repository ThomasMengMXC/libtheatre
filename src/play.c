#include <stdlib.h>
#include <signal.h>

#include "play.h"

static void segfault(int signo);
static int init_ncurses(void);

// left as an exercise for the reader :^)
void init_theatre(void) {
	signal(SIGSEGV, segfault);
	init_ncurses();
	return;
}

static void segfault(int signo) {
	endwin();
	printf("segfaulted: %d\n", signo);
	exit(1);
}

// set up some ncurses specific stuff
static int init_ncurses(void) {
	initscr();

	start_color();
	for (int i = 0; i < 232; i++) {
		init_pair(i, COLOR_BLACK, i);
	}
	cbreak();
	noecho();
	curs_set(0);
	timeout(16); // 60fps baby
	keypad(stdscr, TRUE);
	return 0;
}

int enact_play(Stage *stage) {
	Scene *scene;
	if (stage->currentScene == NULL) {
		return 0;
	}
	stage->currentScene->arrival(stage->currentScene->props);
	int ch = 0;
	int keyboardSignal = -1;
	while(1) {
		ch = getch();
		scene = stage->currentScene; // set the current scene
		if (stage->currentScene) {
			scene->update(scene->props);
			// change scenes base on the return value
			if ((keyboardSignal = scene->keyboard(scene->props, ch)) >= 0) {
				scene_change(stage, keyboardSignal);
			} else if (keyboardSignal == -2) {
				// exit if the signal is -2
				break;
			}
		}
	}
	stage->currentScene->departure(stage->currentScene->props);
	return 0;
}

void finale(void) {
	endwin();
}
