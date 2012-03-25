#include "state_functions.h"

// globals
char computer = CROSSES; //CROSSES is trying to maximize. CROSSES plays first

state initial_state = {
    {{{EMPTY}}},
    {{-1,-1,-1}},
    {-1},
    0
};

const unsigned char display[3] = {' ', 'O', 'X'};

void clear_state(state *s) {
    s->move_number = 0;
    s->most_recent_x = 0;
    s->most_recent_y = 0;
    s->most_recent_z = 0;
    s->heuristic_value = 0;
    memset(&(s->board), EMPTY, SQUARES*sizeof(unsigned char));
}

// Debug printer for states
void prettyprint_state (state s) {
    int x,y,z;
    fprintf(stderr, "Move # %d\t", s.move_number);
    for (x = 0; x < s.move_number; x++) {
        fprintf(stderr, "%2d ", s.moves[x]);
    }
    fprintf(stderr, "\n");
    for(x=0; x<BOARD_DIMENSION; x++) {
        for(y=0; y<BOARD_DIMENSION; y++) {
            fprintf(stderr,"[");
            for(z=0; z<BOARD_DIMENSION; z++)
                fprintf(stderr,"%c,", display[s.board[x][y][z]]);
            fprintf(stderr,"\b]");
            }
        fprintf(stderr,"\n");
        }
    fprintf(stderr,"---------------------------------------------\n");
}

// Debug printer for position_values
void prettyprint_position_values (position_values s) {
    int x,y,z;
    for(x=0; x<BOARD_DIMENSION; x++) {
        for(y=0; y<BOARD_DIMENSION; y++) {
            fprintf(stderr,"[");
            for(z=0; z<BOARD_DIMENSION; z++)
                fprintf(stderr,"%i,", s[x][y][z]);
            fprintf(stderr,"\b]");
            }
        fprintf(stderr,"\n");
        }
    fprintf(stderr,"---------------------------------------------\n");
}

// Generate a deep copy of a state
void replicate(state *s, state *dest) {
    dest->move_number = s->move_number;
    memcpy(&(dest->moves), s->moves, SQUARES*sizeof(int));
    memcpy(&(dest->board), s->board, SQUARES*sizeof(unsigned char));
    memcpy(&(dest->sparse_board), s->sparse_board, SQUARES*sizeof(sparse_array_entry));
}

// Check victory conditions of a state. returns:
// EMPTY: no victory
// NOUGHTS: 'O' player wins
// CROSSES: 'X' player wins
// INVALID: multiple winners; error condition TODO
// ASSUMES: Currently no winner, 4x4x4 board
// (x,y,z) = last move made.
char victory(state s) {
	int x = s.most_recent_x;
	int y = s.most_recent_y;
	int z = s.most_recent_z;
	
    char player = s.board[x][y][z];
    if (!player) return EMPTY;
    int vary, i;
    int invert = BOARD_DIMENSION - 1;

    int lines[13] = {1,1,1,1,1,1,1,1,1,1,1,1,1};
    for (vary=0; vary<BOARD_DIMENSION; vary++) {
        //FLAT LINES (through (x,y,z))
        lines[0]  *= s.board[x][y][vary];
        lines[1]  *= s.board[x][vary][z];
        lines[2]  *= s.board[vary][y][z];
        //EVEN DIAGONALS (through slices containing (x,y,z))
        lines[3]  *= s.board[x][vary][vary];
        lines[4]  *= s.board[vary][y][vary];
        lines[5]  *= s.board[vary][vary][z];
        //ODD DIAGONALS (through slices containing (x,y,z))
        lines[6]  *= s.board[x][invert-vary][vary];
        lines[7]  *= s.board[invert-vary][y][vary];
        lines[8]  *= s.board[invert-vary][vary][z];
        //MAIN DIAGONALS (of whole board)
        lines[9]  *= s.board[vary][vary][vary];
        lines[10] *= s.board[invert-vary][vary][vary];
        lines[11] *= s.board[vary][invert-vary][vary];
        lines[12] *= s.board[vary][vary][invert-vary];
    }
    for (i=0; i<13; i++) if (lines[i] == NOUGHTS_WIN || lines[i] == CROSSES_WIN) return player;
      
    return EMPTY;
}

// get valid succsessors of a state
retval get_successors(state s, char player) {
    state *result = (state *) malloc(sizeof(state)*SQUARES);
    retval ret;
    int i, count;
    int x,y,z;
    for (i = 0; i < SQUARES; i++) {
        clear_state(&result[i]);
        ret.valid[i] = false;
    }
    i = 0;
    count = 0;
    for(x=0; x<BOARD_DIMENSION; x++) {
        for(y=0; y<BOARD_DIMENSION; y++) {
            for(z=0; z<BOARD_DIMENSION; z++) {
                if (s.board[x][y][z] == EMPTY) {
                    count++;
                    replicate(&s, &result[i]);
                    result[i].board[x][y][z] = player;
                    //States carry the most recently played move with them for the victory function
                    result[i].most_recent_x = x;
                    result[i].most_recent_y = y;
                    result[i].most_recent_z = z;
                    //The sparse array is a short list of the coordinates that are non-empty
                    result[i].sparse_board[s.move_number].x = x;
                    result[i].sparse_board[s.move_number].y = y;
                    result[i].sparse_board[s.move_number].z = z;

                    result[i].moves[result[i].move_number] = i;
                    result[i].move_number++;
                    
                    //Heuristic ordering values are calculated
                    if(s.move_number < HEURISTIC_ORDERING_DEPTH) {
                        result[i].heuristic_value = score_state(result[i],map);
                    }
                    
                    ret.valid[i] = true;
                }
                i++;
            }
        }
    }
    //Heuristic ordering values are sorted
    if(s.move_number < HEURISTIC_ORDERING_DEPTH) {
        if(player == computer){
            qsort(result,SQUARES,sizeof(state),compare_states_max_player);
        }else{
            qsort(result,SQUARES,sizeof(state),compare_states_min_player);
        }
    }
    
    
    ret.result = result;
    ret.numsucc = i;

    return ret;
}

//Comparison functions for the quicksort heuristic ordering. 
int compare_states_max_player (const void *a, const void *b) {
    const state *state_a = (const state *) a;
    const state *state_b = (const state *) b;
    if(state_a->heuristic_value > state_b->heuristic_value) {
        return -1;
    }
    if(state_a->heuristic_value < state_b->heuristic_value) {
        return 1;
    }
    return 0;
}
int compare_states_min_player (const void *a, const void *b) {
    const state *state_a = (const state *) a;
    const state *state_b = (const state *) b;
    if(state_a->heuristic_value > state_b->heuristic_value) {
        return 1;
    }
    if(state_a->heuristic_value < state_b->heuristic_value) {
        return -1;
    }
    return 0;
}

//If a state has a victor, returns the win value for that player. Otherwise, returns the heuristic value of the state.
int evaluate(state s, char player) {
    char winner = victory(s);
    if(winner!=EMPTY){
        return (winner==CROSSES)?WIN_SCORE:-WIN_SCORE;
    }else{
        return score_state(s,map);
    }
}

// Uses heuristics to score a state
//Computer winning = positive score, vice versa
// Score is on a scale -304 to 304 (-4*76 to 4*76)
// WONTFIX: make victory INF (MAX_INT)
int score_state(state s, position_values map) {
    int i;
    int x,y,z;
    int score=0;
    for(i=0; i<s.move_number;i++){
        x=s.sparse_board[i].x;
        y=s.sparse_board[i].y;
        z=s.sparse_board[i].z;
        int sign = 0;
        if (s.board[x][y][z] == computer) {
            sign = 1;
        } else if(s.board[x][y][z] != EMPTY) {
            sign = -1;
        }
        int this_position_value = map[x][y][z] * sign;
        score += this_position_value;
    }
    return score;
}

//Calculates from a board whose turn is next
char get_turn(state s){
    int crosses_moves_ahead=0;
    int x,y,z;
    for(x=0; x<BOARD_DIMENSION; x++){
        for(y=0; y<BOARD_DIMENSION; y++){
            for(z=0; z<BOARD_DIMENSION; z++){
                if (s.board[x][y][z] == CROSSES){
                    crosses_moves_ahead++;
                }
                if (s.board[x][y][z] == NOUGHTS){
                    crosses_moves_ahead--;
                }
                if (s.board[x][y][z] == INVALID){
                    return INVALID;
                }
            }
        }
    }
    return (crosses_moves_ahead==1)?NOUGHTS:CROSSES; //Crosses should always be at least one move ahead, as they move first
}

//Fills a 4x4x4 array with values indicating each position's value to the agent
void fill_position_values(position_values map) {
    int x,y,z,victory_pos;
    //Straight victory lines
    for(x=0; x<BOARD_DIMENSION; x++)
        for(y=0; y<BOARD_DIMENSION; y++)
            for(z=0; z<BOARD_DIMENSION; z++)
                map[x][y][z] = 3; //all are included in 3 straight victory lines (X axis, Y axis, Z axis)

    //Diagonal victory lines (X/Z slices)
    for(y=0; y<BOARD_DIMENSION; y++){
        for(victory_pos=0; victory_pos<BOARD_DIMENSION; victory_pos++){
                map[victory_pos][y][victory_pos] ++;
                map[BOARD_DIMENSION-victory_pos-1][y][victory_pos] ++; //two way diagonal (X formation)
         }
    }
    //Diagonal victory lines (Y/Z slices)
    for(x=0; x<BOARD_DIMENSION; x++){
        for(victory_pos=0; victory_pos<BOARD_DIMENSION; victory_pos++){
                map[x][victory_pos][victory_pos] ++;
                map[x][BOARD_DIMENSION-victory_pos-1][victory_pos] ++; //two way diagonal (X formation)
         }
    }
    //Diagonal victory lines (Y/X slices)
    for(z=0; z<BOARD_DIMENSION; z++){
        for(victory_pos=0; victory_pos<BOARD_DIMENSION; victory_pos++){
                map[victory_pos][victory_pos][z] ++;
                map[BOARD_DIMENSION-victory_pos-1][victory_pos][z] ++; //two way diagonal (X formation)
         }
    }
    //The four cross diagonal victory lines
    for(victory_pos=0; victory_pos<BOARD_DIMENSION; victory_pos++){
            map[victory_pos][victory_pos][victory_pos] ++;
            map[BOARD_DIMENSION-victory_pos-1][victory_pos][victory_pos] ++;
            map[victory_pos][BOARD_DIMENSION-victory_pos-1][victory_pos] ++;
            map[victory_pos][victory_pos][BOARD_DIMENSION-victory_pos-1] ++;
    }
    return;
}

//Performs a minimax evaluation on each successor of a state, and returns the best option as the next move
_move pick_next(state s, char player, int depth) {
    retval possible_moves;
    _move retmove;
    int i, score;
    char next_player = (player == CROSSES) ? NOUGHTS : CROSSES;
    int best_score = INT_MIN;
    int best_move;

    possible_moves = get_successors(s, player);
    for (i = 0; i < SQUARES; i++) {
        if (!possible_moves.valid[i]) {
            continue;
        }
        //depth-1 since we are doing the top ply
        score = minimax(possible_moves.result[i], next_player, INT_MIN, INT_MAX, depth-1);
        if (score > best_score) {
            best_score = score;
            best_move = i;
        }
    }
    
    replicate(&possible_moves.result[best_move], &retmove.current);
    retmove.position = best_move;
    retmove.score = best_score;
    
    finalise_retval(&possible_moves);
    return retmove;
}

//Alpha-beta minimax algorithm
int minimax(state s, char player, int alpha, int beta, int depth) {
    int i, v;
    retval moves;
    char next_player;

    next_player = (player == CROSSES) ? NOUGHTS : CROSSES;

    if (depth <= 0 || victory(s) != EMPTY) {
        return evaluate(s, player);
    }

    moves = get_successors(s, player);

    if (player == computer) {
        for (i = 0; i < SQUARES; i++) {
            if (!moves.valid[i]) {
                continue;
            }
            v = minimax(moves.result[i], next_player, alpha, beta, depth-1);
            alpha = max(alpha, v);
            if (beta <= alpha) {
                break;
            }
        }
    } else {
        for (i = 0; i < SQUARES; i++) {
            if (!moves.valid[i]) {
                continue;
            }
            v = minimax(moves.result[i], next_player, alpha, beta, depth-1);
            beta = min(beta, v);
            if (beta <= alpha) {
                break;
            }
        }
    }

    finalise_retval(&moves);
    if (player == computer) {
        return alpha;
    } else {
        return beta;
    }
}

// State I/O

//Sends a reply back to the visualisation program
void send_visual_message(char message[SQUARES]){
    char message_with_terminator[SQUARES+1];
    int i;
    for(i=0; i<SQUARES; i++)
        message_with_terminator[i] = message[i];
    message_with_terminator[SQUARES] = '\0';
    printf("%s",message_with_terminator);
    //This fixes the problem of python hanging on waiting for an EOF character
    fflush(stdout);
}

//Translates a board x y z coordinate into an index for our lists
int coords_to_index(int x,int y,int z){
    return z+y*(BOARD_DIMENSION)+x*(BOARD_DIMENSION*BOARD_DIMENSION);
}

//Converts a character array to a state
void copy_string_to_state(char chars[64], state *output_state){
    int x,y,z;
    for(x=0; x<BOARD_DIMENSION; x++){
        for(y=0; y<BOARD_DIMENSION; y++){
            for(z=0; z<BOARD_DIMENSION; z++){
                int position_content = INVALID;//Invalid by default
                if(chars[coords_to_index(x,y,z)] == 'e')
                    position_content = EMPTY;
                if(chars[coords_to_index(x,y,z)] == 'x')
                    position_content = CROSSES;
                if(chars[coords_to_index(x,y,z)] == 'o')
                    position_content = NOUGHTS;
                if(position_content == INVALID)
                    fprintf(stderr,"[sf:copy_string_to_state]: error, read invalid state %c\n at %d",chars[coords_to_index(x,y,z)],coords_to_index(x,y,z));
                output_state->board[x][y][z] = position_content;
            }
        }
    }
}

//Converts a state to a character array
void copy_state_to_string(state input_state, char chars[64]){
    int x,y,z;
    for(x=0; x<BOARD_DIMENSION; x++){
        for(y=0; y<BOARD_DIMENSION; y++){
            for(z=0; z<BOARD_DIMENSION; z++){
                char position_content = '-';//Invalid by default
                if (input_state.board[x][y][z] == EMPTY)
                    position_content = 'e';
                if (input_state.board[x][y][z] == CROSSES)
                    position_content = 'x';
                if (input_state.board[x][y][z] == NOUGHTS)
                    position_content = 'o';
                chars[coords_to_index(x,y,z)] = position_content;
            }
        }
    }
}

//Places the mark of "player" at x,y,z in the state's board
void apply(state *s, int x, int y, int z, char player) {
    if (s->board[x][y][z] != EMPTY) {
        fprintf(stderr, "[sf:apply]: Square (%d, %d, %d) not empty!\n", x, y, z);
    }
    s->board[x][y][z] = player;
}

