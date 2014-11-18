#include <string.h>
#include <time.h>
#include <time.h>

#include "barman.h"

#define THINK_TIME 5
#define DRINK_TIME 1
#define DRINK_PREPARE_TIME 1
#define NB_DRINKS 5 
#define MAXIMUM_BOTTLES 3

int bar_go_close = 0;

//Argument structure
typedef struct Array_arg {
	int my_id;
	int k_bottles;
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
    int nb_bottles_drink = rand() % MAXIMUM_BOTTLES +1;
	int start = rand() % k_bottles;
	int j, i = start;
	int sum = 0;
    //int array[k_bottles];
    int* array = (int*)malloc(k_bottles*sizeof(int));
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
    if (struct_set_bottles.set_bottles<=0){
       printf("error malloc");
            exit (1);
    }
    printf("\n\n BLUP \n%d\n",struct_set_bottles.set_bottles);
	sum = 0;
    i = 0;
    printf("random choise is: \n");
	while (sum != nb_bottles_drink) { 
		if (array[i] == 1) {
			struct_set_bottles.set_bottles[sum] = i;
            printf("%d ",i);
			sum++;
		}
        i++;
	}
    printf("\n");
	qsort (struct_set_bottles.set_bottles, nb_bottles_drink, sizeof(int), comp);
	
    free(array);
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
	
	int nb_boisson_drank = 0;
	
	while(nb_boisson_drank < NB_DRINKS) {	
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
	
    while(!bar_go_close) {
		
		//Wait a client	
		ready(id_barman);
			
		//Try to take bottles
		take_bottles(id_barman);
		
		//Prepare coktail
		sleep(DRINK_PREPARE_TIME);
		
		//Put bottles
		put_botlles_free_barman(id_barman);
	}
	
	return 0;
}

int main (int argc, char **argv) 
{
     Choice_bottles cc = random_set_bottles (5);

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

	//Seed initialisation
	srand(time(NULL));
	
	//States initialisations
	int *client_states = (int*)malloc(nb_clients * sizeof(int));
	int *bottles_states = (int*)malloc(nb_bottles * sizeof(int));
	int **barman_states = (int**)malloc(nb_barmans * sizeof(int*));
	
	int i;
	for (i = 0; i < nb_barmans; i++) {
		barman_states[i] = (int*)malloc(2*sizeof(int)); 
	}
	
	//Initialisation of array thread arguments
	Array_arg args_c[nb_clients];
	Array_arg args_b[nb_barmans];
	
	//Open bar
	init(nb_clients, nb_barmans, nb_bottles, barman_states, bottles_states, client_states);
	
	
	//Initialisation pthread
	pthread_t *pthreads_clients = (pthread_t *)malloc(nb_clients * sizeof(pthread_t));
	pthread_t *pthreads_barmans = (pthread_t *)malloc(nb_barmans * sizeof(pthread_t));

    for (i = 0; i < nb_barmans; i++) {
        args_b[i].my_id = i;
        args_b[i].k_bottles = nb_bottles;
        if (pthread_create(&pthreads_barmans[i], NULL, f_barman, &args_b[i])) {

                printf("ERROR !\n");
                return -1;
        }
    }

	for (i = 0; i < nb_clients; i++) {
		args_c[i].my_id = i;
		args_c[i].k_bottles = nb_bottles;
        if (pthread_create(&pthreads_clients[i], NULL, f_client, &args_c[i])) {

				printf("ERROR !\n");
				return -1;
        }
	}
	
    //Wait all threads
    for (i = 0; i < nb_clients; i++) {
		pthread_join(pthreads_clients[i], NULL);
	}

    bar_go_close = 1;

    for (i = 0; i < nb_barmans; i++) {
		pthread_join(pthreads_barmans[i], NULL);
	}
	
	//The bar close
	bar_close();
	
	printf("\n** The bar is close **\n");
    
    free(pthreads_clients);
    free(pthreads_barmans);
    free(client_states);
    free(bottles_states); 
	for (i = 0; i < nb_barmans; i++) {
		free(barman_states[i]); 
	}
	free(barman_states);
	
	return 0;
}
  
