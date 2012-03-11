#define BOARD_DIMENSIONS 4
#define EMPTY   0
#define NOUGHTS 1
#define CROSSES 2
#define INVALID 3

#include <stdio.h>
#include <stdlib.h>

typedef char state[BOARD_DIMENSIONS][BOARD_DIMENSIONS][BOARD_DIMENSIONS];

state initial_state = {EMPTY};
