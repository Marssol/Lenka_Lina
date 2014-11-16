#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>


typedef struct Array_arg{
	int *array;
	int array_size;
}Array_arg;

int search(int *T, int n, int x)
{
	int i;
	for (i = 0; i < n; i++) {
		if (T[i] == x) {
			return (i+1);
		}
	}
	return -1;
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

	if (argc != 3) {
		fprintf(stderr, "usage : %s size array and element to search\n", argv[0]);
		exit (-1) ;
	}

	//Initialisation of the data
	Array_arg arg;
	arg.array_size = atoi(argv[1]);
	arg.array = (int *)malloc(arg.array_size * sizeof(int));

	//Initialisation of the array
	int i;
	for (i = 0; i < arg.array_size; i++) {
		arg.array[i] = i + 1;
	}
	shuffle(arg.array, arg.array_size);

	//Value we would like find
	int toSearch = atoi(argv[2]);

	struct timeval timeBegin;
	gettimeofday(&timeBegin, NULL);
	long int t1 = timeBegin.tv_sec*1000000 + timeBegin.tv_usec;

	if( search(arg.array, arg.array_size, toSearch) != -1)
	{
		printf("The element %d is in the index %d\n", toSearch, search(arg.array, arg.array_size, toSearch));
	}
	else
	{
		printf("The element %d isn't in the array\n", toSearch);
	}
	gettimeofday(&timeBegin, NULL);
	long int t2 = timeBegin.tv_sec*1000000 + timeBegin.tv_usec;

	printf("%ld microseconds have passed\n", t2-t1);

	free(arg.array);

	return 0;

}

