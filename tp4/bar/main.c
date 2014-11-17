#include <string.h>
#include <time.h>
#include <time.h>

#include "barman.h"

#define THINK_TIME 5
#define DRINK_TIME 1
#define DRINK_PREPARE_TIME 1
#define NB_DRINKS 5 
#define MAXIMUM_BOTTLES 3

//Argument structure
typedef struct Array_arg {
	int my_id;
	int k_bottles
} Array_arg;

int comp (const void * elem1, const void * elem2) 
{
    int f = *((int*)elem1);
    int s = *((int*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

Choice_bottles random_set_bottles (int k_bottles)
{
	int nb_bottles_drink = rand() % MAXIMUM_BOTTLES;
	int start = rand() % k_bottles;
	int j, i = start;
	int sum = 0;
	int array[k_bottles];
	for (j = 0; j < k_bottles; j++) {
		array[j] = 0;
	}
	while (sum != nb_bottles_drink) {
		if (rand() % 2 == 1) {
			if (array[i] != 1) {
				array[i] = 1;
				sum++;
			}
		}
		if (i == k_bottles) {
			i = 0;
		}
		else {
			i++;
		}
	}
	
	Choice_bottles struct_set_bottles;
	struct_set_bottles.nb_bottles = nb_bottles_drink;
	struct_set_bottles.set_bottles = (int *) malloc(nb_bottles_drink * sizeof(int));
	sum = 0;
	i = 0;
	while (sum != nb_bottles_drink) { 
		if (array[i] == 1) {
			struct_set_bottles.set_bottles[sum] = i;
			sum++;
		}
		i++;
	}
	
	qsort (struct_set_bottles.set_bottles, nb_bottles_drink, sizeof(int), comp);
	
	return struct_set_bottles; 
} 

void free_bottles(Choice_bottles c) 
{
	free(c.set_bottles);
}

void *f_client(void *arg) 
{	
	Array_arg *argu = (Array_arg *)arg;
	int id_client = argu->my_id;
	int k_bottles = argu->k_bottles;
	
	//list of bottles
	Choice_bottles str_set_bot;
	
	while(nb_boisson_drank < NB_DRINK_DRINKING) {	
		//Time for thinking
		int think_time = rand() % THINK_TIME;
		sleep(think_time);
		
		//Generate randomly list of bottles
		str_set_bot = random_set_bottles (k_bottles);
		
        take_barman(id_client, str_set_bot);
		
		//Drink
		sleep(DRINK_TIME);
		
		nb_boisson_drank++;
		free_bottles(str_set_bot);
	}
	
	return 0;
}

void * f_barman(void *arg) 
{
	Array_arg *argu = (Array_arg *)arg;
	int id_barman = argu->my_id;
	int k_bottles = argu->k_bottles;
	
	while(!bar_close) {
		
		//Wait a client	
		ready(id_barman);
			
		//Try to take bottles
		take_bottles(id_barman);
		
		//Prepare coktail
		sleep(time Coktail);
		
		//Put bottles
		put_botlles(nb_barman);
	}
	sleep(clean_bar);
	
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
  
