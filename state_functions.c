#include "state_functions.h"

int main(void) {
	computer = CROSSES;

	succ_demo();
	//pick_demo();
	

	return EXIT_SUCCESS;
}

void pick_demo() {
	pick_next(initial_state, computer, 2);
}

void succ_demo() {
	position_values map;
	fill_position_values(map);
	
	retval plies[10];
	int i = 0;

	plies[0] = get_successors(initial_state, NOUGHTS);
	plies[1] = get_successors(plies[0].result[0], CROSSES);
	plies[2] = get_successors(plies[1].result[0], NOUGHTS);
	plies[3] = get_successors(plies[2].result[0], CROSSES);
	plies[4] = get_successors(plies[3].result[0], NOUGHTS);
	plies[5] = get_successors(plies[4].result[0], CROSSES);
	plies[6] = get_successors(plies[5].result[0], NOUGHTS);
	plies[7] = get_successors(plies[6].result[0], CROSSES);
	plies[8] = get_successors(plies[7].result[0], NOUGHTS);
	
	
	for (i = 0; i < plies[8].numsucc; i++) {
		prettyprint_state(plies[8].result[i]);
		int score = score_state(plies[8].result[i],map);
		fprintf(stderr,"[sf:succ_demo] State score: %i \n",score);
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

// Debug printer for position_values
void prettyprint_position_values (position_values s) {
	int x,y,z;
	for(x=0; x<BOARD_DIMENSIONS; x++) {
		for(y=0; y<BOARD_DIMENSIONS; y++) {
			printf("[");
			for(z=0; z<BOARD_DIMENSIONS; z++)
				printf("%i,", s[x][y][z]);
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

//Uses heuristics to score a state
int score_state(state s, position_values map) {
	//Noughts winning = positive score, Crosses winning = negative score
	//Score is on a scale -304 to 304 (-4*76 to 4*76)
	//TODO; normalize this score so it can be used in place of a victory evaluation if there is no victor
	int x,y,z;
	int score=0;
	for(x=0; x<BOARD_DIMENSIONS; x++){
		for(y=0; y<BOARD_DIMENSIONS; y++){
			for(z=0; z<BOARD_DIMENSIONS; z++){
				int sign = 0;
				if(s[x][y][z] == NOUGHTS){
					sign = 1;
				}else if(s[x][y][z] == CROSSES){
					sign = -1;
				}
				int this_position_value = map[x][y][z] * sign;
				score += this_position_value;
			}
		}
	}
	return score;
}

//Fills a 4x4x4 array with values indicating each position's value to the agent
void fill_position_values(position_values map) {
	int x,y,z,victory_pos;
	//Straight victory lines
	for(x=0; x<BOARD_DIMENSIONS; x++)
		for(y=0; y<BOARD_DIMENSIONS; y++)
			for(z=0; z<BOARD_DIMENSIONS; z++)
				map[x][y][z] = 3; //all are included in 3 straight victory lines (X axis, Y axis, Z axis)
				
	//Diagonal victory lines (X/Z slices)
	for(y=0; y<BOARD_DIMENSIONS; y++){
		for(victory_pos=0; victory_pos<BOARD_DIMENSIONS; victory_pos++){
            	map[victory_pos][y][victory_pos] ++;
            	map[BOARD_DIMENSIONS-victory_pos-1][y][victory_pos] ++; //two way diagonal (X formation)
         }
    }
    //Diagonal victory lines (Y/Z slices)
    for(x=0; x<BOARD_DIMENSIONS; x++){
		for(victory_pos=0; victory_pos<BOARD_DIMENSIONS; victory_pos++){
            	map[x][victory_pos][victory_pos] ++;
            	map[x][BOARD_DIMENSIONS-victory_pos-1][victory_pos] ++; //two way diagonal (X formation)
         }
    }
    //Diagonal victory lines (Y/X slices)
    for(z=0; z<BOARD_DIMENSIONS; z++){
		for(victory_pos=0; victory_pos<BOARD_DIMENSIONS; victory_pos++){
            	map[victory_pos][victory_pos][z] ++;
            	map[BOARD_DIMENSIONS-victory_pos-1][victory_pos][z] ++; //two way diagonal (X formation)
         }
    }
    //The four cross diagonal victory lines
    for(victory_pos=0; victory_pos<BOARD_DIMENSIONS; victory_pos++){
        	map[victory_pos][victory_pos][victory_pos] ++;
        	map[BOARD_DIMENSIONS-victory_pos-1][victory_pos][victory_pos] ++;
        	map[victory_pos][BOARD_DIMENSIONS-victory_pos-1][victory_pos] ++;
        	map[victory_pos][victory_pos][BOARD_DIMENSIONS-victory_pos-1] ++;
    }
	return;
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

