#include "state_functions.h"
#include <unistd.h>
#include <signal.h>

extern char computer;

int main(int argc, char *argv[])
{
    fill_position_values(map);

    state read_state;
    _move next_move;

    clear_state(&read_state);

    next_move = pick_next(read_state, get_turn(read_state), DEPTH);
    return 0;
}
