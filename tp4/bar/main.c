#include <string.h>
#include <time.h>
#include <time.h>

#include "philo.h"

#define THINK_TIME 5
#define DRINK_TIME 1
#define PAYMENT_TIME 1
#define NB_MEALS 5 

//Argument structure
typedef struct Array_arg {
	int nb_philo;
} Array_arg;



void *f (void *arg) 
{
	Array_arg *argu = (Array_arg *)arg;
	return 0;
}

int main (int argc, char **argv) 
{
	if (argc != 4) {
		fprintf(stderr, "usage : %s number of clients, barmans and botles\n", argv[0]);
		exit (-1) ;
	}
	
	//Variables
	//Check if arguments are corrects
    int nb_clients = atoi(argv[1]);
	if (nb_clients < 1) {
		printf("Number of clients have to be minimum 1\n");
		return(-1);
	}
    int nb_barmans = atoi(argv[2]);
	if (nb_barmans < 1) {
		printf("Number of barmans have to be minimum 1\n");
		return(-1);
	}
    int nb_bottles = atoi(argv[3]);
	if (nb_bottles < 1) {
		printf("Number of bottles have to be minimum 1\n");
		return(-1);
	}
/*	
	//Seed initialisation
	srand(time(NULL));
	
	//Initialisation state philosophes
	philo_state = (int*)malloc(nb_thread * sizeof(int));
	
	//Initialisation of array thread arguments
	Array_arg args[nb_thread];
	
	//Dinner beginning
	init(nb_thread, philo_state);
	
	//Initialisation pthread
	int i;
	pthread_t *pthreads = (pthread_t *)malloc(nb_thread*sizeof(pthread_t));

    //Initialisation mutex
    pthread_mutex_init (&mut, NULL);

	for (i = 0; i < nb_thread; i++) {
		args[i].nb_philo = i;
        if (pthread_create(&pthreads[i], NULL, dinner, &args[i])) {

				printf("ERROR !\n");
				return -1;
        }
	}
	
    //Wait all threads
    for (i = 0; i < nb_thread; i++) {
		pthread_join(pthreads[i], NULL);
	}
	
	dinner_terminate();
	
	printf("\n** The dinner is terminate **\n");
    
    free(pthreads);
    free(philo_state);
*/
	return 0;
}
  
