#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define MAX 100000000
#define MIN 1

typedef struct Array_arg{
	int *array;
	int array_size;
	int begin;
	int end;
	int x;
}Array_arg;

int found;

void *search(void *argument)
{
	Array_arg *arg = (Array_arg *)argument;
	int i, value;
	for(i = arg->begin; (i < arg->begin + arg->end) && !found; i++)
	{
		if(arg->array[i] == arg->x)
		{
			found = 1;
			value = i;
			pthread_exit((void*) value);
		}
	}
	value = -1;
	pthread_exit((void*) value);
}

//Function shuffle randomly an array
void shuffle(int *array, int n) {
	int i, j, tmp;
	for (i = n - 1; i > 0; i--) {
		j = rand() % (i+1);
		tmp = array[j];
		array[j] = array[i];
		array[i] = tmp;
	}
}

int main (int argc, char **argv){

	if (argc != 4){
		fprintf(stderr, "usage : %s size of array, element to search and number of threads\n", argv[0]);
		exit (-1) ;
	}

	int nbThread = atoi(argv[3]);
	if (nbThread > (atoi(argv[1])/ 2)) {
		printf("Number of thread is greater to array size divided by 2\n");
		return -1;
	}

	Array_arg arg;
	arg.array_size = atoi(argv[1]);
	arg.x = atoi(argv[2]);
	arg.array = (int *)malloc(arg.array_size * sizeof(int));

	//Initialisation array
	int i;
	for (i = 0; i < arg.array_size; i++) {
		arg.array[i] = (i + 1);
	}
	shuffle(arg.array, arg.array_size);

	//Value search
	arg.x = atoi(argv[2]);

	//Initialisation Thread
	pthread_t *table_pthread = malloc(nbThread*sizeof(pthread_t));
	int repartition = arg.array_size / nbThread;
	int add = 0;
	if (repartition * nbThread != arg.array_size) {
		add = arg.array_size - (repartition * nbThread);
	}

	//Begin timer
	struct timeval timeBegin;
	gettimeofday(&timeBegin, NULL);
	long int t1 = timeBegin.tv_sec*1000000 + timeBegin.tv_usec;

	//Initialisation boolean : found 
	found = 0;

	for (i=0; i< nbThread; i++) {
		Array_arg *tmp = malloc(sizeof(Array_arg));
		tmp->begin = i * repartition;
		tmp->end = repartition;
		tmp->array = arg.array;
		tmp->array_size = arg.array_size;
		tmp->x = arg.x;
		if (i == nbThread -1) {
			tmp->end += add;
		}
		if (pthread_create(&table_pthread[i], NULL, search, tmp) < 0) {
			printf("ERROR !\n");
			return -1;
		}
	}

	//Values return by threads
	int values[nbThread];

	//Wait until every thread end
	int j;
	for (j = 0; j < nbThread; j ++) {
		pthread_join(table_pthread[j], (void**) &values[j]);
	}

	//End timer
	gettimeofday(&timeBegin, NULL);
	long int t2 = timeBegin.tv_sec*1000000 + timeBegin.tv_usec;

	//Print index au the value and the time for searching
	for (j = 0; j < nbThread; j++) {
		if (values[j] != -1) {
			//printf("The value is at index: %d\n", values[j]);
		}
	}

	//printf("%ld microseconds have passed\n", t2-t1);
	printf("%ld ", t2-t1);
	
	//Free memory
	free(table_pthread);
	free(arg.array);

	return 0;

}

