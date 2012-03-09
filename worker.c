#include <stdio.h>
#include <unistd.h>
#define LAST 10
  
int main(int argc, char *argv[])
{
	char str [80];
	scanf ("%s",str);
	printf("Read the line %s",str);
	return 0;
}

