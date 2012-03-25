#include "state_functions.h"
#include <unistd.h>
#include <signal.h>

#define OPTCHARS "d:"

extern char computer;

struct global_options_t {
    int search_depth;
} global_options;

void parse_args(int argc, char *argv[]) {
    extern char *optarg;
    extern int optind;
    extern int opterr;
    char optchar;

    int depth = DEFAULT_DEPTH;

    while ((optchar = getopt(argc, argv, OPTCHARS)) != -1) {
        switch (optchar) {
            case 'd':
                if (sscanf(optarg, "%d", &depth) != 1) {
                    fprintf(stderr, "Please enter a numeric value for depth\n");
                    fprintf(stderr, "usage: %s [-d <depth>]\n", argv[0]);
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                fprintf(stderr, "usage: %s [-d <depth>]\n", argv[0]);
                exit(EXIT_FAILURE);
                break;
        }
    }

    if (depth < 0 || depth > SQUARES) {
        fprintf(stderr, "warning: invalid value for depth, using %d\n", DEFAULT_DEPTH);
        depth = DEFAULT_DEPTH;
    }

    global_options.search_depth = depth;
}


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

    parse_args(argc, argv);

    // XXX: fix this thing
    scanf("%d %d %d", &x, &y, &z);

    while (1)
    {
        //copy_string_to_state(read_state_string, &read_state);
        prettyprint_state(read_state);
        //ply = get_successors(read_state, get_turn(read_state));
        next_move = pick_next(read_state, get_turn(read_state), global_options.search_depth);
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
