#include "barman.c"

//Global variables
int *clients_states; 
int **barman_states; 
int *bottle_states; 

int nb_barman;
int nb_client;

Choice_bottles *client_choices;

pthread_cond_t  cv_all_barmans;  
pthread_cond_t  *cv_clients;  
pthread_cond_t  *cv_barmans;  
pthread_cond_t  *cv_bottles; 
pthread_mutex_t m_barmans; 
pthread_mutex_t m_bottles; 

void init(int n_clients, int m_barmans, int k_bottles, int *states_barmans, int *states_bottles, int *states_clients)
{
}

void take_barman(int id_client, Choice_bottles str_set_bottles)
{	
	pthread_mutex_lock (&m_barmans);  
 
	client_state[id_client] = WAITING;
	//Show states tables
	printf("Client wait a barman \n",id_client);
	print_client();
   
	//Check if one barman is free
	int i = 0;
    while (state_barman[i] != DISPONIBLE) {
		if (i == nb_barman) {
			pthread_cond_wait (&cv_all_barmans,&m_barmans);
		}
		i++;
	}
	
	barman_state[i][0] = WAITING;
	barman_state[i][1] = id_client;
	client_state[nb_client] = SERVED;
	
	//The client choice is saved
	client_choices[id_client] = str_set_bottles;
	pthread_cond_signal(cv_barmans[i]);
	
	while (barman_state[i][0] != DISPONIBLE){
		pthread_cond_wait(cv_client[i],&m_barmans);
	}
	
	//Show states tables
	
	printf("Client %d ask his drinking choice\n",id_client);
	print();
	client_state[nb_client] = DRINKING;
	pthread_mutex_unlock (&m_barmans);
}


void ready(int id_barman)
{
	pthread_mutex_lock (&m_barmans);  
	while (state_barman[id_barman][0] == DISPONIBLE) {
		pthread_cond_wait(cv_barmans[id_barman],&m_barmans);
	}
		
	pthread_mutex_unlock (&m_barmans);
}

void take_bottles(int id_barman)
{
	pthread_mutex_lock (&m_bottles);  
   
	//Check if nb_bottles are free
	int i;
	int my_id_client = barman_states[id_barman][1]];
	int *set_bottles = client_choices[my_id_client].set_bottles;
	
	for (i = 0; i < client_choices[my_id_client].nb_bottles; i++) {
		while (bottle_states[set_bottles[i]] != DISPONIBLE) {
			pthread_cond_wait (&cv_bottles[set_bottles[i]],&m_bottles);
		}
		
		bottle_states[set_bottles[i]] = UNAVAILABLE;
	}
	
	barman_state[id_barman][0] = BUSY;
	
	//Show states tables
	printf("Barman %d prepare the coktail\n",nb_philo);
	print();
	
	pthread_mutex_unlock (&m_bottles);	
}

void put_bottles_free_barman(int id_barman)
{
	pthread_mutex_lock (&m_bottles);
	
    //Prevent your client he can drink
	pthread_cond_signal (&cv_clients[barman_state[id_barman][1]]);
	
	//Prevent all you collegue the botlles are avaible
	int i;
	int my_id_client = barman_states[id_barman][1]];
	int *set_bottles = client_choices[my_id_client].set_bottles;
	
	for (i = 0; i < client_choices[my_id_client].nb_bottles; i++) {
		pthread_cond_signal (&cv_bottles[set_bottles[i]]);	
	}
	
	
	pthread_mutex_lock (&m_barmans);  
	barman_state[id_barman][0] = READY;
	//Show states tables
	printf("Barman %d finish prepare the drink, he is ready\n",id_barman);
	print();
	pthread_mutex_unlock (&m_barmans);  
	
	pthread_mutex_unlock (&m_bottles);  
}

void bar_close(void)
{
}
