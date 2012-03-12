#define BOARD_DIMENSIONS 4
#define SQUARES BOARD_DIMENSIONS*BOARD_DIMENSIONS*BOARD_DIMENSIONS
#define EMPTY   0
#define NOUGHTS 1
#define CROSSES 2
#define INVALID 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char state[BOARD_DIMENSIONS][BOARD_DIMENSIONS][BOARD_DIMENSIONS];

state initial_state = {EMPTY};
unsigned char display[3] = {' ', 'O', 'X'};

// Debug printer for states
void prettyprint_state (state s) {
    int x,y,z;
    for(x=0; x<BOARD_DIMENSIONS; x++) {
        for(y=0; y<BOARD_DIMENSIONS; y++) {
            printf("[");
            for(z=0; z<BOARD_DIMENSIONS; z++)
                printf("%c,", display[s[x][y][z]]);
            printf("\b]");
            }
        printf("\n");
        }
    printf("---------------------------------------------\n");
}

// Generate a deep copy of a state
void replicate(state s, char player, state *dest) {
    memcpy(dest, s, sizeof(*dest));
}

// Check victory conditions of a state. returns:
// EMPTY: no victory
// NOUGHTS: 'O' player wins
// CROSSES: 'X' player wins
// INVALID: multiple winners; error condition
char victory(state s) {
    return 0;
}

// get valid succsessors of a state, terminated with INVALID in bottom top left
state *get_successors(state s, char player) {
    state *result = malloc(sizeof(state)*SQUARES);
    int i=0;
    int x,y,z;
    for(x=0; x<BOARD_DIMENSIONS; x++)
        for(y=0; y<BOARD_DIMENSIONS; y++)
            for(z=0; z<BOARD_DIMENSIONS; z++)
                if (s[x][y][z] == EMPTY) {
                    replicate(s,player,&result[i]);
                    result[i++][x][y][z] = player;
                    //if (victory(result[i-1])) 
                    //  {handle victory conditions, or report error state}
                }
    result[i][0][0][0] = INVALID; //TODO: better way to terminate
    return result;
}

int main(void)
{
    state *next;
    int i = 0;

    next = get_successors(initial_state, NOUGHTS);
    next = get_successors(next[0], CROSSES);
    next = get_successors(next[1], NOUGHTS);
    next = get_successors(next[2], CROSSES);
    next = get_successors(next[3], NOUGHTS);
    next = get_successors(next[4], CROSSES);
    next = get_successors(next[5], NOUGHTS);
    while (next[i][0][0][0] != INVALID)
        prettyprint_state(next[i++]);

    return EXIT_SUCCESS;
}
