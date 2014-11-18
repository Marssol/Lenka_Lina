#include "barman.h"

//Global variables
int *clients_states; 
int **barman_states; 
int *bottle_states; 

int nb_barman;
int nb_client;
int nb_bottles;

Choice_bottles *client_choices;

//Condition
pthread_cond_t  cv_all_barmans;  
pthread_cond_t  *cv_clients;  
pthread_cond_t  *cv_barmans;  
pthread_cond_t  *cv_bottles; 

//Mutex
pthread_mutex_t mb;  //Mutex barman
pthread_mutex_t mbo; //Mutex bottles

static void print() 
{
	printf("\nKLI bar is open\n");
	int i;
	printf("\n* Clients *\n");
	for (i = 0; i < nb_client; i++) {
		switch(clients_states[i]) {
			case THINKING : 
				printf("T ");
				break;
			case DRINKING : 
				printf("D ");
				break;
			case WAITING : 
				printf("W ");
				break;
			case SERVED : 
				printf("S ");
				break;
			default :
				break;
		}
	}
	printf("\n* Barmans *\n");
	for (i = 0; i < nb_barman; i++) {
		switch(barman_states[i][0]) {
            case WAITING :
                printf("W ");
				break;
			case BUSY : 
				printf("B ");
				break;
			case DISPONIBLE : 
				printf("D ");
				break;
			default :
				break;
		}
	}
	printf("\n* Bottles *\n");
	for (i = 0; i < nb_bottles; i++) {
		switch(bottle_states[i]) {
			case AVAILABLE : 
				printf("A ");
				break;
			case UNAVAILABLE : 
				printf("U ");
				break;
			default :
				break;
		}
	}
	printf("\n_______________\n");	
}

void init(int n_clients, int m_barmans, int k_bottles, int **states_barmans, int *states_bottles, int *states_clients)
{
	pthread_mutex_init (&mb, NULL);
	pthread_mutex_init (&mb, NULL);
	clients_states = states_clients;
	barman_states = states_barmans;
	bottle_states = states_bottles;
	
	cv_clients = (pthread_cond_t*)(malloc(n_clients*sizeof(pthread_cond_t)));
	cv_barmans = (pthread_cond_t*)(malloc(m_barmans*sizeof(pthread_cond_t)));
	cv_bottles = (pthread_cond_t*)(malloc(k_bottles*sizeof(pthread_cond_t)));

    client_choices = (Choice_bottles*)(malloc(n_clients*sizeof(Choice_bottles)));
	
	//Initialisation pthread_cond
	pthread_cond_init (&cv_all_barmans, NULL);

	int i;
    for (i = 0; i < m_barmans; i++) {
        pthread_cond_init (&cv_barmans[i], NULL);
        barman_states[i][0] = DISPONIBLE;
        barman_states[i][1] = -1;
    }
	for (i = 0; i < n_clients; i++) {
		pthread_cond_init (&cv_clients[i], NULL);
		clients_states[i] = THINKING;
    }
	for (i = 0; i < k_bottles; i++) {
        pthread_cond_init (&cv_bottles[i], NULL);
        bottle_states[i] = AVAILABLE;
    }
	nb_client = n_clients;
	nb_barman = m_barmans;
	nb_bottles = k_bottles;
	print();
}

void take_barman(int id_client, Choice_bottles str_set_bottles)
{	
	pthread_mutex_lock (&mb);  
 
	clients_states[id_client] = WAITING;
	//Show states tables
	printf("Client %d wait a barman \n",id_client);
	print();
   
	//Check if one barman is free
	int i = 0;
    while (barman_states[i][0] != DISPONIBLE && i < nb_barman ){
        if (i == nb_barman - 1) {
			pthread_cond_wait (&cv_all_barmans,&mb);
            i = 0;
		}
		i++;
	}

    barman_states[i][0] = WAITING;
    barman_states[i][1] = id_client;
	clients_states[nb_client] = SERVED;
	
	//The client choice is saved
	client_choices[id_client] = str_set_bottles;
	pthread_cond_signal(&cv_barmans[i]);
	
	while (barman_states[i][0] != DISPONIBLE){
		pthread_cond_wait(&cv_clients[i],&mb);
	}
	
	//Show states tables
	
    printf("Client %d drink\n",id_client);
	print();
	clients_states[nb_client] = DRINKING;
	pthread_mutex_unlock (&mb);
}


void ready(int id_barman)
{
	pthread_mutex_lock (&mb);  
	while (barman_states[id_barman][0] == DISPONIBLE) {
		pthread_cond_wait(&cv_barmans[id_barman],&mb);
	}
		
	pthread_mutex_unlock (&mb);
}

void take_bottles(int id_barman)
{
	pthread_mutex_lock (&mbo);  
   
	//Check if nb_bottles are free
	int i;
	int my_id_client = barman_states[id_barman][1];
	int *set_bottles = client_choices[my_id_client].set_bottles;
	
	for (i = 0; i < client_choices[my_id_client].nb_bottles; i++) {
		while (bottle_states[set_bottles[i]] != DISPONIBLE) {
			pthread_cond_wait (&cv_bottles[set_bottles[i]],&mbo);
		}
		
		bottle_states[set_bottles[i]] = UNAVAILABLE;
	}
	
	barman_states[id_barman][0] = BUSY;
	
	//Show states tables
	printf("Barman %d prepare the coktail\n",id_barman);
	print();
	
	pthread_mutex_unlock (&mbo);	
}

void put_botlles_free_barman(int id_barman)
{
	pthread_mutex_lock (&mbo);
	
    //Prevent your client he can drink
	pthread_cond_signal (&cv_clients[barman_states[id_barman][1]]);
	
	//Prevent all you collegue the botlles are avaible
	int i;
	int my_id_client = barman_states[id_barman][1];
	int *set_bottles = client_choices[my_id_client].set_bottles;
	
	for (i = 0; i < client_choices[my_id_client].nb_bottles; i++) {
		pthread_cond_signal (&cv_bottles[set_bottles[i]]);	
	}
	
	
	pthread_mutex_lock (&mb);  
	barman_states[id_barman][0] = DISPONIBLE;
	//Show states tables
	printf("Barman %d finish prepare the drink, he is ready\n",id_barman);
	print();
	pthread_mutex_unlock (&mb);  
	
	pthread_mutex_unlock (&mbo);  
}

void bar_close(void)
{
	free(cv_clients);
	free(cv_barmans);
	free(cv_bottles);
}
