#include <string.h>
#include <time.h>
#include <time.h>

#include "philo.h"

#define THINK_TIME 5
#define EAT_TIME 1
#define NB_MEALS 5 

//Argument structure
typedef struct Array_arg {
	int nb_philo;
} Array_arg;

//Global variables
int *philo_state;

void *dinner (void *arg) 
{
	Array_arg *argu = (Array_arg *)arg;
    int nb_philo = argu->nb_philo;
    int nb_meals_ate = 0;
    
    while (nb_meals_ate < NB_MEALS) {
	
		//Time for thinking
		int think_time = rand() % THINK_TIME;
		sleep(think_time);
		
        take_chopstick(nb_philo);
		
		//Eating
		sleep(EAT_TIME);
		
		nb_meals_ate++;
		
        put_chopstick(nb_philo);
	}
	
	return 0;
}

int main (int argc, char **argv) 
{
	if (argc != 2) {
		fprintf(stderr, "usage : %s number of threads\n", argv[0]);
		exit (-1) ;
	}
	
	//Variables
	//Check is nb_thread is correct
    int nb_thread = atoi(argv[1]);
	if (nb_thread > 5) {
		printf("Number of thread have to be minimum 5\n");
		return(-1);
	}
	
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

	return 0;

}
  
