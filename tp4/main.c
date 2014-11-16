#include <string.h>
#include <time.h>
#include <time.h>

#include "philo.h"

#define MAX 12
#define EAT_TIME 5


//Global variables
int end_dinner;
int *philo_state;

void *dinner (void *arg) 
{
	int nb_philo = *((int *)nb_philo);

	while (!end_dinner) {
	
		//Time for thinking
		int think_time = rand() % MAX;
		sleep(think_time);
		
		take_chopstick(nb_philo);
		
		//Eating
		sleep(EAT_TIME);
		
		put_chopstick(nb_philo);
	}
}

int main (int argc, char **argv) 
{
	if (argc != 2) {
		fprintf(stderr, "usage : %s number of threads\n", argv[0]);
		exit (-1) ;
	}
	
	//Variables
	//Check is nb_thread is correct
	int nb_thread = atoi(argv[2]);
	if (nb_thread > 5) {
		printf("Number of thread have to be minimum 5\n");
		return(-1);
	}
	
	//Seed initialisation
	srand(time(NULL));
	
	//Initialisation state philosophes
	philo_state = (int*)malloc(nb_thread * sizeof(int));
	
	//Dinner beginning
	end_dinner = 0;
	init(nb_thread, philo_state);
	
	//Initialisation pthread
	int i;
	pthread_t *pthreads = (pthread_t *)malloc(nb_thread*sizeof(pthread_t));
	for (i = 0; i < nb_thread; i++) {
		if (pthread_create(&pthreads[i], NULL, dinner, &i)) {
				printf("ERROR !\n");
				return -1;
		}
	}
	
	//Wait all threads
	for (i = 0; i < nb_thread; i++) {
		pthread_join(pthreads[i], NULL);
	}

	free(pthreads);
	free(philo_state);
	
	return 0;
}
  
