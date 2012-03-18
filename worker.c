#include "state_functions.h"
#include <unistd.h>
#include <signal.h>
#define DEPTH 2


int main(int argc, char *argv[])
{
    int move_number = 0;
    
    position_values map;
    fill_position_values(map);
    
    
    //state state1;
    //retval state1_ply;
    //_move state1_next_move;
    //fprintf(stderr,"\n");
    //copy_string_to_state("eeeeooeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeexxx", state1);
    
    //state1_ply = get_successors(state1, get_turn(state1));
    //state1_next_move = pick_next(state1, get_turn(state1), DEPTH);
    //prettyprint_state(state1);
    //prettyprint_state(state1_ply.result[state1_next_move.position]);
    

    while (1)
    {
    
        char read_state_string [64];
        scanf ("%s",read_state_string);
        
        //Printing to stderr will show output on the terminal window. Use this to write debug messages in the terminal
        fprintf(stderr,"[worker] received board state: %s from visualiser\n",read_state_string);
        
        
        state read_state;
        _move next_move;
        char next_state_string [64];
        retval ply;
        
        copy_string_to_state(read_state_string,read_state);
        ply = get_successors(read_state, get_turn(read_state));
        next_move = pick_next(read_state, get_turn(read_state), DEPTH);
        
        
        copy_state_to_string(ply.result[next_move.position],next_state_string); // Just pick the first as a test
        
        
        
        //Pause
        sleep(1);
        fprintf(stderr,"[worker] %s output sending\n",next_state_string);
        send_visual_message(next_state_string);
        move_number++;
    }
    return 0;
}
