#ifndef _STATE_FUNCTIONS_H
#define _STATE_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define BOARD_DIMENSIONS 4
#define SIDE BOARD_DIMENSIONS
#define SQUARES BOARD_DIMENSIONS*BOARD_DIMENSIONS*BOARD_DIMENSIONS
#define EMPTY   0
#define NOUGHTS 1
#define CROSSES 2
#define INVALID 3

typedef struct {
    unsigned char board[SIDE][SIDE][SIDE];
    int cross_score;
    int nought_score;
} _state;

typedef struct {
    int position;
    int score;
} _move;

typedef unsigned char state[BOARD_DIMENSIONS][BOARD_DIMENSIONS][BOARD_DIMENSIONS];

typedef struct _retval {
    state *result;
    int numsucc;
} retval;

// prototypes
void prettyprint_state (state s);
void replicate(state s, char player, state *dest);
char victory(state s);
retval get_successors(state s, char player);
_move pick_next(state s, char player, int depth);
int minimax(state s, char player, int alpha, int beta, int depth);
int evaluate(state s, char player);

void pick_demo();
void succ_demo();

// globals
char computer;
state initial_state = {{{EMPTY}}};
const unsigned char display[3] = {' ', 'O', 'X'};

// inlines
static inline int max(int x, int y) {
    return (x > y) ? x : y;
}

static inline int min(int x, int y) {
    return (x < y) ? x : y;
}

#endif

