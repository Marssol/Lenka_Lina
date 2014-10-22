#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MAX 2
#define MIN 1

typedef struct Array_arg{
	int *array;
	int arraySize;
	int begin;
	int end;
}Array_arg;

void *sum (void *argument) {
	Array_arg *arg = (Array_arg *)argument;
	int i;
	int sum = 0;
	printf("Begin : %d\n", arg->begin);
	printf("End : %d\n", arg->end);
	for(i = arg->begin; i < arg->begin + arg->end; i++){
		sum += arg->array[i];
	}
	return (void *) sum;
}

int main (int argc, char **argv){

if (argc != 3){
	fprintf(stderr, "usage : %s size of array and number of threads\n", argv[0]);
    exit (-1) ;
}


int nbThread = atoi(argv[2]);
if (nbThread > (atoi(argv[1])/ 2)) {
	return(-1);
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

//Initialisation pthread
pthread_t *table_pthread = malloc(nbThread*sizeof(pthread_t));
int repartition = array.arraySize / nbThread;
int add = 0;
if (repartition * nbThread != array.arraySize) {
	add = array.arraySize - (repartition * nbThread);
}
for (i = 0; i < nbThread; i++) {
	array.begin = i * repartition;
	array.end = repartition;
	if (i == nbThread - 1) {
		array.end += add;
	}
	pthread_create(&table_pthread[i], NULL, sum, &array);
}

int sum = 0;
int sub_sum = 0;
//WAit until every thread end
for (i = 0; i < nbThread; i++) {
	pthread_join(table_pthread[i], (void**) &sub_sum);
	sum += sub_sum;
	sub_sum = 0;
}


printf("The sum is equal to %d\n", sum);

free(table_pthread);

return 0;

}

