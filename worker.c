#include "state_functions.h"
#include <unistd.h>
#include <signal.h>
#define DEPTH 2


int main(int argc, char *argv[])
{
    int move_number = 0;
    
    position_values map;
    fill_position_values(map);

    state read_state;
    _move next_move;
    char next_state_string [64];
    retval ply;
    char read_state_string [64];

    clear_state(&read_state);

    while (1)
    {
    
        scanf("%s", read_state_string);
        
        //Printing to stderr will show output on the terminal window. Use this to write debug messages in the terminal
        fprintf(stderr,"[worker] received board state: %s from visualiser\n",read_state_string);
        
        
        
        copy_string_to_state(read_state_string, &read_state);
        ply = get_successors(read_state, get_turn(read_state));
        next_move = pick_next(read_state, get_turn(read_state), DEPTH);
        
        
        copy_state_to_string(ply.result[next_move.position],next_state_string); // Just pick the first as a test
        
        
        
        //Pause
        sleep(1);
        fprintf(stderr,"[worker] %s output sending\n",next_state_string);
        send_visual_message(next_state_string);

        finalise_retval(&ply);
        move_number++;
    }
    return 0;
}
