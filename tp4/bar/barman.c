#include "barman.h"

//Global variables
int closed;

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
pthread_mutex_t mClose;

static void print() 
{
    //printf("\nKLI bar is open\n");
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
            case DISPONSIBLE :
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
void finishDrink(int id)
{
    clients_states[id] = THINKING;
}

void init(int n_clients, int m_barmans, int k_bottles, int **states_barmans, int *states_bottles, int *states_clients, int* bar_go_close)
{
    closed = 0;
	pthread_mutex_init (&mb, NULL);
	pthread_mutex_init (&mb, NULL);
    pthread_mutex_init (&mClose, NULL);
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
        barman_states[i][0] = DISPONSIBLE;
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
    int i = 0; // index of barman
    while (i < nb_barman && barman_states[i][0] != DISPONSIBLE  ){
        if (i == nb_barman - 1) {
			pthread_cond_wait (&cv_all_barmans,&mb);
            i = -1;
		}
		i++;
	}

    // ith barman is free. 0 = state of barman, 1 = id client
    barman_states[i][0] = WAITING;
    //sleep(3);
    barman_states[i][1] = id_client;
    clients_states[id_client] = SERVED;
    printf("Client %d have a barman %d now\n",id_client,i);
    print();
	//The client choice is saved
    client_choices[id_client] = str_set_bottles;

    pthread_cond_signal(&cv_barmans[i]); // signal barman serve drink to client
    printf("sent signal to barman %d\n",i);
    //if (barman_states[i][0] != DISPONIBLE){
        pthread_cond_wait(&cv_clients[id_client],&mb); // client is waiting for his/her drink
        printf("DRINK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    //}
	//Show states tables
    clients_states[id_client] = DRINKING;
    printf("Client %d drink\n",id_client);
	print();

	pthread_mutex_unlock (&mb);
}

// barman has nothing to do
int ready(int id_barman)
{
    pthread_mutex_lock (&mb);
    //pthread_mutex_lock (&mClose);


    if (barman_states[id_barman][0] == WAITING)
    {
        pthread_mutex_unlock (&mb);
        //pthread_mutex_unlock (&mClose);
        return closed;
    }
    while (barman_states[id_barman][0] == DISPONSIBLE && !closed) {
        //printf("barman will wait\n");
        pthread_cond_wait(&cv_barmans[id_barman],&mb); // waiting for job
	}
    //pthread_mutex_unlock (&mClose);
	pthread_mutex_unlock (&mb);
    return closed;
}

void go_home()
{
    pthread_mutex_lock (&mb);
    closed = 1;
    int i;
    for(i = 0;i<nb_barman;i++)
    {
        pthread_cond_signal(&cv_barmans[i]);
    }
    pthread_mutex_unlock (&mb);
}




void take_bottles(int id_barman)
{
    pthread_mutex_lock (&mbo);
   
	//Check if nb_bottles are free
    int i; // index of bottle in the set of clients choice
	int my_id_client = barman_states[id_barman][1];
	int *set_bottles = client_choices[my_id_client].set_bottles;
    printf("bottles chosen by client %d and wanted by barman %d\n",my_id_client,id_barman);
    for (i = 0; i < client_choices[my_id_client].nb_bottles; i++)
    {
        printf("%d ",set_bottles[i]);
    }
    printf("\n");
	for (i = 0; i < client_choices[my_id_client].nb_bottles; i++) {
        while (bottle_states[set_bottles[i]] != AVAILABLE) {
            pthread_cond_wait (&cv_bottles[set_bottles[i]],&mbo); // barman waiting for bottle i
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
    //pthread_cond_signal (&cv_clients[barman_states[id_barman][1]]);
	
    //announce all you collegue the botlles are avaible
    int i; // index of bottles choices
	int my_id_client = barman_states[id_barman][1];
	int *set_bottles = client_choices[my_id_client].set_bottles;
	
	for (i = 0; i < client_choices[my_id_client].nb_bottles; i++) {
        bottle_states[set_bottles[i]] = AVAILABLE;
        pthread_cond_signal (&cv_bottles[set_bottles[i]]); // telling other barmans that bottle is free
	}
    pthread_mutex_unlock (&mbo);
	
    pthread_mutex_lock (&mb); // lock barmans
    barman_states[id_barman][0] = DISPONSIBLE;
    pthread_cond_signal (&cv_clients[my_id_client]); // hey, client, your drink is ready!
	//Show states tables
	printf("Barman %d finish prepare the drink, he is ready\n",id_barman);
	print();
    //adding -lenka
    pthread_cond_signal (&cv_all_barmans); // barman is free
    pthread_mutex_unlock (&mb);
}

void bar_close(void)
{
	free(cv_clients);
	free(cv_barmans);
	free(cv_bottles);
    free(client_choices);
}
