#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

typedef struct Array_arg {
	int *vect_1;
	int *vect_2;
	int array_size;
	int begin;
	int interval;
} Array_arg;

void *scalar(void *argument)
{
	Array_arg *arg = (Array_arg *)argument;
	int i;
	int sum = 0;
	for(i = arg->begin; i < +arg->begin + arg->interval; i++)
	{
		sum += arg->vect_1[i] * arg->vect_2[i];
	}
	pthread_exit((void*) sum);
}

int main (int argc, char **argv)
{
	if (argc != 3) {
		fprintf(stderr, "usage : %s size of array and number of threads\n", argv[0]);
		exit (-1) ;
	}

	//Check if all thread can be usefull
	int nb_thread = atoi(argv[2]);
	if (nb_thread > (atoi(argv[1])/ 2)) {
		printf("Number of thread is greater to array size divided by 2\n");
		return(-1);
	}

	//Argument initialisation
	Array_arg arg;
	arg.array_size = atoi(argv[1]);
	arg.vect_1 = (int *)malloc(arg.array_size * sizeof(int));
	arg.vect_2 = (int *)malloc(arg.array_size * sizeof(int));

	//Array generate by the user
	int i;
	//Vector 1 :
	printf("Enter le values of the first array : \n");
	for (i = 0; i < arg.array_size; i++) {
		scanf("%d",&arg.vect_1[i]);
	}	
	//Vector 2 :
	printf("\nEnter le values of the second array : \n");
	for (i = 0; i < arg.array_size; i++) {
		scanf("%d",&arg.vect_2[i]);
	}
	printf("\n");

	//Initialisation pthread
	pthread_t *pthreads = malloc(nb_thread*sizeof(pthread_t));
	int interval = arg.array_size / nb_thread;
	int add = 0;
	if (interval * nb_thread != arg.array_size) {
		add = arg.array_size - (interval * nb_thread);
	}
	Array_arg args[nb_thread];

	for (i = 0; i < nb_thread; i++) {
		args[i].begin = i * interval;
		args[i].interval = interval;
		args[i].vect_1 = arg.vect_1;
		args[i].vect_2 = arg.vect_2;
		args[i].array_size = arg.array_size;
	
		if (i == nb_thread - 1) {
			args[i].interval += add;
		}
		if( pthread_create(&pthreads[i], NULL, scalar, &args[i]) < 0) {
			printf("ERROR !\n");
			return -1;
		}
	}

	int j, sum = 0;
	int sub_sum[nb_thread];
	
	//Wait until every thread interval
	for (j = 0; i < nb_thread; j++) {
		pthread_join(pthreads[j], (void**) &sub_sum[j]);
		sum += sub_sum[j];
	}

	printf("( ");
	for(i = 0; i < arg.array_size; i++) {
		printf("%d ", arg.vect_1[i]);
	}
	printf(" ) . ( ");
	for(i = 0; i < arg.array_size; i++) {
		printf("%d ", arg.vect_2[i]);
	}
	printf(" ) = %d\n", sum);
	
	free(arg.vect_1);
	free(arg.vect_2);
	free(pthreads);

	return 0;

}
