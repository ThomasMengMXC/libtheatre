#ifndef PLAY_H
#define PLAY_H

#include "stage.h"

void init_theatre(void);
int init_ncurses(void);
int enact_play(Stage *stage);

#endif
