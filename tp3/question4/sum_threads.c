#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define MAX 5
#define MIN 3

//Argument structure
typedef struct Array_arg {
	int *array;
	int array_size;
	int begin;
	int interval;
} Array_arg;

//Global variables
int global_sum;
pthread_mutex_t lock;

void *sum (void *argument) 
{
	Array_arg *arg = (Array_arg *)argument;
	int i, sum = 0;
	
	//printf("interval [%d - %d] \n",arg->begin,arg->begin + arg->interval);
	
	for (i = arg->begin; i < arg->begin + arg->interval; i++) {
		sum += arg->array[i];
	}

	pthread_mutex_lock(&lock); //Lock
	global_sum += sum;
	pthread_mutex_unlock(&lock); //Unlock

	pthread_exit(NULL);
}

int main (int argc, char **argv)
{
	if (argc != 3) {
		fprintf(stderr, "usage : %s size of array and number of threads\n", argv[0]);
		exit (-1) ;
	}

	//Check is nb_thread is usefull
	int nb_thread = atoi(argv[2]);
	if (nb_thread > (atoi(argv[1])/ 2)) {
		printf("Number of thread is greater to array size divided by 2\n");
		return(-1);
	}

	global_sum = 0;
	if (pthread_mutex_init(&lock, NULL) != 0) {
		printf("Mutex init failed\n");
	}

	//Argument initialisation
	Array_arg arg;
	arg.array_size = atoi(argv[1]);
	arg.array = (int *)malloc(arg.array_size * sizeof(int));
	
	//Seed initialisation
	srand(time(NULL));

	//Array randomly generate
	int i;
	for (i = 0; i < arg.array_size; i++) {
		arg.array[i] = rand() % MAX + MIN ;
		// printf("array[%d] = %d \n",i,array.array[i]);
	}

	//Initialisation pthread
	pthread_t *pthreads = malloc(nb_thread*sizeof(pthread_t));
	int interval = arg.array_size / nb_thread;
	int add = 0;
	if (interval * nb_thread != arg.array_size) {
		add = arg.array_size - (interval * nb_thread);
	}
	Array_arg args[nb_thread];

	//Array management divided by thread
	int y;
	for (y = 0; y < nb_thread; y++) {
		//Thread argument
		args[y].begin = y * interval;
		args[y].interval = interval;
		args[y].array = arg.array;
		args[y].array_size = arg.array_size;

		//The last thread
		if (y == nb_thread - 1) {
			args[y].interval += add;
		}
		
		if( pthread_create(&pthreads[y], NULL, sum, &args[y]) < 0) {
			printf("ERROR !\n");
			return -1;
		}
	}

	int j;
	//Wait until every thread interval
	for (j = 0; j < nb_thread; j++) {
		pthread_join(pthreads[j], NULL);
	}

	printf("\n Value of array :\n");
	for(i = 0; i < arg.array_size; i++){
		printf("%d\n", arg.array[i]);
	}

	printf("\n Value of global :\n");
	printf("%d\n", global_sum);

	free(pthreads);
	free(arg.array);
	pthread_mutex_destroy(&lock);

	return 0;
}

