#ifndef _STATE_FUNCTIONS_H
#define _STATE_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define BOARD_DIMENSION 4
#define SIDE BOARD_DIMENSION
#define SQUARES BOARD_DIMENSION*BOARD_DIMENSION*BOARD_DIMENSION
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

typedef unsigned char state[BOARD_DIMENSION][BOARD_DIMENSION][BOARD_DIMENSION];
typedef int position_values[BOARD_DIMENSION][BOARD_DIMENSION][BOARD_DIMENSION];

typedef struct _retval {
    state *result;
    int numsucc;
} retval;

// prototypes
void prettyprint_state (state s);
void prettyprint_position_values (position_values s);
void replicate(state s, char player, state *dest);
char victory(state s, int x, int y, int z);

retval get_successors(state s, char player);

_move pick_next(state s, char player, int depth);
int minimax(state s, char player, int alpha, int beta, int depth);
int evaluate(state s, char player);
void fill_position_values(position_values scoremap);
int score_state(state s, position_values map);
char get_turn(state s);


void pick_demo();
void succ_demo();

// State I/O
void send_visual_message(char message[SQUARES]);
void copy_string_to_state(char chars[SQUARES], state output_state);
void copy_state_to_string(state input_state, char chars[SQUARES]);
int coords_to_index(int x,int y,int z);



// inlines
static inline int max(int x, int y) {
    return (x > y) ? x : y;
}

static inline int min(int x, int y) {
    return (x < y) ? x : y;
}

#endif

