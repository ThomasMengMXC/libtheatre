#include <signal.h>
#include <time.h>

#include "play.h"

static void segfault(int signo);
static int init_ncurses(void);

/* left as an exercise for the reader :^) */
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

/* set up some ncurses specific stuff */
static int init_ncurses(void) {
	int i;
	initscr();

	start_color();
	for (i = 0; i < 233; i++) {
		init_pair(i + 1, COLOR_BLACK, i);
	}
	cbreak();
	noecho();
	curs_set(0);
	timeout(0);
	keypad(stdscr, TRUE);
	return 0;
}

int enact_play(Stage *stage) {
	Scene *scene;
	int ch, keyboardSignal = -1;
	time_t t0, t1;
	if (!stage->currentScene) return 0;
	stage->currentScene->arrival(stage->currentScene->props);
	while(1) {
		t0 = clock();
		scene = stage->currentScene;
		scene->update(scene->props);
		/* change scenes base on the return value */
		while ((ch = getch()) != ERR) {
			if ((keyboardSignal = scene->keyboard(scene->props, ch)) >= 0) {
				scene_change(stage, keyboardSignal);
				scene = stage->currentScene;
			}
		}
		if (keyboardSignal == -2) break;
		t1 = clock();
		napms(17 - (t1 - t0) * 1000/CLOCKS_PER_SEC);
	}
	stage->currentScene->departure(stage->currentScene->props);
	return 0;
}

void finale(void) {
	endwin();
}
