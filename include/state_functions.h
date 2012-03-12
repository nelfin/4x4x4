#ifndef _STATE_FUNCTIONS_H
#define _STATE_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_DIMENSIONS 4
#define SQUARES BOARD_DIMENSIONS*BOARD_DIMENSIONS*BOARD_DIMENSIONS
#define EMPTY   0
#define NOUGHTS 1
#define CROSSES 2
#define INVALID 3

typedef unsigned char state[BOARD_DIMENSIONS][BOARD_DIMENSIONS][BOARD_DIMENSIONS];

typedef struct _retval {
    state *result;
    int numsucc;
} retval;

void prettyprint_state (state s);
void replicate(state s, char player, state *dest);
char victory(state s);
retval get_successors(state s, char player);

#endif

