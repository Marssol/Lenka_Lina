#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100
#define MIN 3

typedef struct Array_arg {
	int *array;
	int array_size;
}Array_arg;

int sum(Array_arg arg) 
{
	int i, sum = 0;
	for (i = 0; i < arg.array_size; i++) {
		sum += arg.array[i];
	}
	
	return sum;
}

int main (int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "usage : %s size of array\n", argv[0]);
		exit (-1) ;
	}

	//Arguments initialisation
	Array_arg arg;
	arg.array_size = atoi(argv[1]);
	arg.array = (int*)malloc(arg.array_size * sizeof(int));
	
	//Seed initialisation
	srand(time(NULL));
	
	//Array randomly generated
	int i, total;
	for (i = 0; i < arg.array_size; i++) {
		arg.array[i] = rand() % MAX + MIN ;
		// printf("array[%d] = %d \n",i,arg.array[i]);
	}
	
	//Calculate sum
	total = sum(arg);
	
	printf("Array sum is %d.\n", total);
	
	return 0;
}

