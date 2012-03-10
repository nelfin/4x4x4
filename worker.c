#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
	//while (1)
	//{
		char board_state [64];
		scanf ("%s",board_state);
		
		//Printing to stderr will show output on the terminal window. Use this to write debug messages in the terminal
		fprintf(stderr,"Worker read board state: %s\n",board_state);
		sleep(3);
		//A dummy state to return (no algorithm actually implemented here)
		printf("xxxoeeeeeeexeeeeeeeeeeeoooeeeeeeeeeeeeeeeeeeeexxeeeeeeeeeeeeeeee\n");
		fprintf(stderr,"Output sent\n");
		sleep(3);
		fprintf(stderr,"Worker shutting down\n");
		
	//}
	return 0;
}
