#include "state_functions.h"
#include <unistd.h>
#include <signal.h>


int main(int argc, char *argv[])
{
	fprintf(stderr,"MADE");
	int move_number = 0;
	
	position_values map;
	fill_position_values(map);
	
	
	while (1)
	{
	
		char read_state_string [64];
		scanf ("%s",read_state_string);
		
		//Printing to stderr will show output on the terminal window. Use this to write debug messages in the terminal
		fprintf(stderr,"[worker] received board state: %s from visualiser\n",read_state_string);
		
		
		state read_state;
		char next_state_string [64];
		retval ply;
		
		copy_string_to_state(read_state_string,read_state);
		ply = get_successors(read_state,get_turn(read_state));
		
		copy_state_to_string(ply.result[0],next_state_string); // Just pick the first as a test
		prettyprint_state(ply.result[0]);
		
		//Simulate calculation
		sleep(1);
		fprintf(stderr,"[worker] %s output sending\n",next_state_string);
		send_visual_message(next_state_string);
		move_number++;
	}
	return 0;
}
