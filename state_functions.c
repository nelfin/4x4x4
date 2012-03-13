#include "state_functions.h"

int main(void) {
    computer = CROSSES;

    //succ_demo();
    pick_demo();

    return EXIT_SUCCESS;
}

void pick_demo() {
    pick_next(initial_state, computer, 2);
}

void succ_demo() {
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
}

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

    fprintf(stderr, "[sf:get_successors] found %d successors\n", i);

    return ret;
}

int evaluate(state s, char player) {
    // TODO; actually score the board
    return (int) player;
}

_move pick_next(state s, char player, int depth) {
    retval possible_moves;
    _move retmove;
    int i, score;
    int best_score = INT_MIN;
    int best_move;

    possible_moves = get_successors(s, player);
    for (i = 0; i < possible_moves.numsucc; i++) {
        score = minimax(s, player, INT_MIN, INT_MAX, depth);
        if (score > best_score) {
            best_score = score;
            best_move = i;
        }
        fprintf(stderr,
                "[sf:pick_next] minimax(%02d) = %d\n", i, score);
    }

    retmove.position = best_move;
    retmove.score = best_score;
    return retmove;
}

int minimax(state s, char player, int alpha, int beta, int depth) {
    int i, v;
    retval moves;
    char next_player;

    next_player = (player == CROSSES) ? NOUGHTS : CROSSES;

    if (depth <= 0) {
        return evaluate(s, player);
    }

    moves = get_successors(s, next_player);

    if (player == computer) {
        for (i = 0; i < moves.numsucc; i++) {
            v = minimax(moves.result[i], next_player, alpha, beta, depth-1);
            alpha = max(alpha, v);
            if (beta <= alpha) {
                break;
            }
        }
        return alpha;
    } else {
        for (i = 0; i < moves.numsucc; i++) {
            v = minimax(moves.result[i], next_player, alpha, beta, depth-1);
            beta = min(beta, v);
            if (beta <= alpha) {
                break;
            }
        }
        return beta;
    }
}

