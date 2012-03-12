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

typedef struct _retval {
    state *result;
    int numsucc;
} retval;

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
retval get_successors(state s, char player) {
    state *result = malloc(sizeof(state)*SQUARES);
    retval ret;
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
    ret.result = result;
    ret.numsucc = i;
    return ret;
}

int main(void)
{
    retval plies[10];
    int i = 0;

    plies[0] = get_successors(initial_state, NOUGHTS);
    plies[1] = get_successors(plies[0].result[0], CROSSES);
    plies[2] = get_successors(plies[1].result[0], NOUGHTS);
    plies[3] = get_successors(plies[2].result[0], CROSSES);
    plies[4] = get_successors(plies[3].result[0], NOUGHTS);
    plies[5] = get_successors(plies[4].result[0], CROSSES);
    plies[6] = get_successors(plies[5].result[0], NOUGHTS);
    for (i = 0; i < plies[6].numsucc; i++) {
        prettyprint_state(plies[6].result[i]);
    }

    for (i = 0; i < 7; i++) {
        free(plies[i].result);
    }

    return EXIT_SUCCESS;
}
