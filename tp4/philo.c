#include "philo.h"

#define LEFT (nb_philo + philos - 1) % philos 
#define RIGHT (nb_philo + 1) % philos        

//Global variables
int *philos_state; 
int philos;
pthread_cond_t  *cv;   
pthread_mutex_t m;      

static void print() 
{
	printf("\nDinner states\n");
	int i;
	for (i = 0; i < philos; i++) {
		switch(philos_state[i]) {
			case THINKING : 
				printf("T ");
				break;
			case EATING : 
				printf("E ");
				break;
			case HUNGRY : 
				printf("H ");
				break;
			default :
				break;
		}
	}
	printf("\n_______________\n");	
}         


void init(int nb_philos, int *states) 
{	
	pthread_mutex_init (&m, NULL);
	philos_state = states;
	cv = (pthread_cond_t*)(malloc(nb_philos*sizeof(pthread_cond_t)));
	
	int i;
	for (i = 0; i < nb_philos; i++) {
		pthread_cond_init (&cv[i], NULL);
		philos_state[i] = THINKING;
	}
	
	philos = nb_philos;
	print();
}

void take_chopstick(int nb_philo) 
{
    printf("%d im here\n",nb_philo);
	pthread_mutex_lock (&m);  
   
	philos_state[nb_philo] = HUNGRY;
	//Show states tables
	printf("Philosophe %d is hungry\n",nb_philo);
	print();
   
	//Check left and right is free
    while (LEFT == EATING || RIGHT == EATING) {
		pthread_cond_wait (&cv[nb_philo],&m);
	}
	
	philos_state[nb_philo] = EATING;
	
	//Show states tables
	printf("Philosophe %d start to eat\n",nb_philo);
	print();
	
	pthread_mutex_unlock (&m);  
}

void put_chopstick(int nb_philo)
{
	pthread_mutex_lock (&m); 
	
	philos_state[nb_philo] = THINKING;
	//Show states tables
	printf("Philosophe %d finish eat and start to think\n",nb_philo);
	print();
	
	
    //Prevent your neitghbour you finish eating
	pthread_cond_signal (&cv[LEFT]);
	pthread_cond_signal (&cv[RIGHT]);
	
	pthread_mutex_unlock (&m);   
}
