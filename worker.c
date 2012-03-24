#include "state_functions.h"
#include <unistd.h>
#include <signal.h>

extern char computer;

int main(int argc, char *argv[])
{
    int move_number = 0;
    
    fill_position_values(map);

    state read_state;
    _move next_move;
    char next_state_string [64];
    retval ply;
    char read_state_string [64];

    int x, y, z;
    clear_state(&read_state);

    // XXX: fix this thing
    scanf("%d %d %d", &x, &y, &z);

    while (1)
    {
        //copy_string_to_state(read_state_string, &read_state);
        prettyprint_state(read_state);
        //ply = get_successors(read_state, get_turn(read_state));
        next_move = pick_next(read_state, get_turn(read_state), DEPTH);
        prettyprint_state(next_move.current);

        replicate(&next_move.current, &read_state);

        copy_state_to_string(next_move.current, next_state_string); // Just pick the first as a test
        
        fprintf(stderr,"[worker] %s output sending\n",next_state_string);
        send_visual_message(next_state_string);

        scanf("%d %d %d", &x, &y, &z);
        apply(&read_state, x, y, z, (computer == CROSSES) ? NOUGHTS : CROSSES);

        //finalise_retval(&ply);
        move_number++;
    }
    return 0;
}
