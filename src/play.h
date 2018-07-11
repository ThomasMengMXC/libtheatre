#ifndef PLAY_H
#define PLAY_H
// This is referring to the theatre play, NOT play game

#include "stage.h"

#ifdef __cplusplus
extern "C" {
#endif
void init_theatre(void);
int enact_play(Stage *stage);
void finale(void);
#ifdef __cplusplus
}
#endif

#endif
