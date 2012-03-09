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
		
		//A dummy state to return (no algorithm actually implemented here)
		printf("xxxoeeeeeeexeeeeeeeeeeeoooeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee\n");
	//}
	return 0;
}
