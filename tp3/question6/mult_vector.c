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

int scalar(int *vec_1, int *vec_2, int n)
{
	int i;
	int scal = 0;
	for (i = 0; i < n; i++) {
		scal += vec_1[i] * vec_2[i];
	}
	return scal;
}

int main (int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "usage : %s size of array\n", argv[0]);
		exit (-1) ;
	}

	//Argument initialisation
	Array_arg arg_1;
	Array_arg arg_2;
	arg_1.array_size = atoi(argv[1]);
	arg_1.array = (int *)malloc(arg_1.array_size * sizeof(int));
	arg_2.array_size = atoi(argv[1]);
	arg_2.array = (int *)malloc(arg_2.array_size * sizeof(int));

	//Array generate by the user
	int i;
	//Vector 1 :
	printf("Enter values of the first array : \n");
	for (i = 0; i < arg_1.array_size; i++) {
		scanf("%d",&arg_1.array[i]);
	}
	//Vector 2 :
	printf("\nEnter values of the second array : \n");
	for (i = 0; i < arg_2.array_size; i++) {
		scanf("%d",&arg_2.array[i]);
	}
	
	//Show values
	printf("\n");
	printf("( ");
	for (i = 0; i < arg_1.array_size; i++) {
		printf("%d ", arg_1.array[i]);
	}
	printf(" ) * ( ");
	for (i = 0; i < arg_2.array_size; i++) {
	printf("%d ", arg_2.array[i]);
	}

	printf(" ) = %d\n", scalar(arg_1.array, arg_2.array, arg_1.array_size));

	free(arg_1.array);
	free(arg_2.array);
	
	return 0;

}
