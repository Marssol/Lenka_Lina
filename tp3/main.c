#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100
#define MIN 3

typedef struct Array_arg{
	int *array;
	int arraySize;
}Array_arg;

int sum (Array_arg arg) {

	int i;
	int sum = 0;
	for(i = 0; i < arg.arraySize; i++){
		sum += arg.array[i];
	}
	return sum;
}

int main (int argc, char **argv){

if (argc != 2){
	fprintf(stderr, "usage : %s size of array\n", argv[0]);
    exit (-1) ;
}

Array_arg array;
array.arraySize = atoi(argv[1]);
array.array = (int *)malloc(array.arraySize * sizeof(int));
srand(time(NULL));
int i;
for (i = 0; i < array.arraySize; i++) {
	array.array[i] = rand() % MAX + MIN ;
	printf("array[%d] = %d \n",i,array.array[i]);
}

printf("The sum is equal to %d\n", sum(array));
return 0;

}

