#ifndef _STATE_FUNCTIONS_H
#define _STATE_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#define BOARD_DIMENSION 4
#define SIDE BOARD_DIMENSION
#define SQUARES BOARD_DIMENSION*BOARD_DIMENSION*BOARD_DIMENSION
#define EMPTY   0
#define NOUGHTS 1
#define CROSSES 2
#define INVALID 3
#define MAX_STATIC_SCORE ((3*BOARD_DIMENSION*BOARD_DIMENSION) + 3*BOARD_DIMENSION*2)*BOARD_DIMENSION
#define WIN_SCORE MAX_STATIC_SCORE+1 //As long as this is greater than the maximum static value (304 = BOARD_DIMENSION * Number of victory lines)
#define CROSSES_WIN 16
#define NOUGHTS_WIN NOUGHTS
#define DEPTH 6

typedef struct _sparse_array_entry {
    int x;
    int y;
    int z;
} sparse_array_entry;

typedef struct _state {
    unsigned char board[SIDE][SIDE][SIDE];
    //Sparse array is only valid up to move_number. After that there is no guarantee on what the values are
    sparse_array_entry sparse_board[SQUARES];
    int moves[SQUARES];
    int move_number;
    int most_recent_x;
    int most_recent_y;
    int most_recent_z;
} state;

typedef struct {
    state current;
    int position;
    int score;
} _move;

typedef int position_values[BOARD_DIMENSION][BOARD_DIMENSION][BOARD_DIMENSION];

typedef struct _retval {
    state *result;
    bool valid[SQUARES];
    int numsucc;
} retval;



// prototypes
void prettyprint_state (state s);
void prettyprint_position_values (position_values s);
void replicate(state *s, state *dest);
char victory(state s);
char get_any_victory(state s);

retval get_successors(state s, char player);

_move pick_next(state s, char player, int depth);
int minimax(state s, char player, int alpha, int beta, int depth);
int evaluate(state s, char player);
void fill_position_values(position_values scoremap);
char get_turn(state s);
int score_state(state s, position_values map);
//For the score_state function, filled in worker.c
position_values map;


void pick_demo();
void succ_demo();

// State I/O
void send_visual_message(char message[SQUARES]);
void copy_string_to_state(char chars[SQUARES], state *output_state);
void copy_state_to_string(state input_state, char chars[SQUARES]);
int coords_to_index(int x,int y,int z);
void clear_state(state *s);
void apply(state *s, int x, int y, int z, char player);

// inlines
static inline int max(int x, int y) {
    return (x > y) ? x : y;
}

static inline int min(int x, int y) {
    return (x < y) ? x : y;
}

static inline void finalise_retval(retval *r) {
    free(r->result);
}

#endif

