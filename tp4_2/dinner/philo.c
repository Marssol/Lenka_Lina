#include "philo.h"

//Global variables
//Philosophes states
int *philos_state;
//Nb philosophes 
int philos;
//Condition
pthread_cond_t  *cv;  
//Mutex 
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
	//Mutex initialisation
	pthread_mutex_init (&m, NULL);
	
	philos_state = states;
	
	//Conditions initialisation
	cv = (pthread_cond_t*)(malloc(nb_philos*sizeof(pthread_cond_t)));
	int i;
	for (i = 0; i < nb_philos; i++) {
		pthread_cond_init (&cv[i], NULL);
		philos_state[i] = THINKING;
	}
	
	philos = nb_philos;
	print();
}

void take_chopstick(int id_philo) 
{
	pthread_mutex_lock (&m);  
	
	int left = (id_philo + philos - 1) % philos;
	int right = (id_philo + 1) % philos;
   
	philos_state[id_philo] = HUNGRY;
	//Show states tables
	printf("Philosophe %d is hungry\n",id_philo);
	print();
   
	//Check left and right is free
    while (philos_state[left] == EATING || philos_state[right] == EATING) {
		pthread_cond_wait (&cv[id_philo],&m);
	}
	
	philos_state[id_philo] = EATING;
	
	//Show states tables
	printf("Philosophe %d start to eat\n",id_philo);
	print();
	
	pthread_mutex_unlock (&m);  
}

void put_chopstick(int id_philo)
{
	pthread_mutex_lock (&m);
	
	int right = (id_philo + 1) % philos; 
	
	philos_state[id_philo] = THINKING;
	//Show states tables
	printf("Philosophe %d finish eat and start to think\n",id_philo);
	print();
	
	
    //Prevent your neitghbour you finish eating
	pthread_cond_signal (&cv[right]);
	
	pthread_mutex_unlock (&m);   
}

void dinner_terminate(void)
{
	free(cv);
}
