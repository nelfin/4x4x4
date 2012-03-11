/* vim: set ts=4: */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char *argv[])
{
	int i = 0;
	while (1)
	{
	
		char board_state [64];
		scanf ("%s",board_state);
		
		//Printing to stderr will show output on the terminal window. Use this to write debug messages in the terminal
		fprintf(stderr,"[worker] received board state: %s from visualiser\n",board_state);
		
		//Simulate calculation
		sleep(3);
		
		//A dummy state to return (no algorithm actually implemented here)
		//Modulates to show that the loop is working
		if(i%2==0){
		printf("xxxoeeeeeeexeeeeeeeeeeeoooeeeeeeeeeeeeeeeeeeeexxeeeeeeeeeeeeeeee");
		}else{
		printf("ooooeeeeeeeeeeeeeeeeeeeoeoeexxxeeeeeeeeeeeeeeexeeeeeeoooeeexxeee");
		}
		//This fixes the problem of python hanging on waiting for an EOF character
		fflush(stdout);
		
		fprintf(stderr,"[worker] Dummy output sent\n");
		i++;
	}
	return 0;
}
