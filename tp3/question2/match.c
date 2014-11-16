#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#define NB_SONG 3

//Structure arguments 
typedef struct Arg_thread {
	char* chant;
	int freq;
} Arg_thread;

//Fonction call by thread
void *supporter (void *arg)
{
	Arg_thread *str = (Arg_thread *)arg;
	int i, pid;
	pthread_t tid;
	  
	pid = getpid();
	tid = pthread_self();
	
	//Seed initialisation
	srand((int)tid);

	for (i = 0; i < str->freq; i++) {
		printf("Processus %d Thread %x : %s \n", pid, (unsigned int)tid, str->chant);
		usleep(rand() / RAND_MAX * 1000000.);
	}
	
	return (void *) tid ;
}

int main (int argc, char **argv)
{
	if (argc != 5) {
		fprintf(stderr, "usage : %s team1 team2 frequency 1 frequency 2\n", argv[0]);
		exit(-1);
	}
  
	//Declarations variables
	int team1 ;
	int team2 ;
	int i ;
	int nb_threads = 0;
	pthread_t *tids;

	//Initialisation variables
	team1 = atoi(argv[1]);
	team2 = atoi(argv[2]);
	Arg_thread arg_team1;
	arg_team1.freq = atoi(argv[3]);
	arg_team1.chant = "Allons enfants de la patrie";
	Arg_thread arg_team2;
	arg_team2.freq = atoi(argv[4]);
	arg_team2.chant = "Swing low, sweet chariot";

	nb_threads = team1 + team2; 
	tids = malloc (nb_threads*sizeof(pthread_t)) ;

	/* Create the threads for team1 */
	for (i = 0 ; i < team1; i++){
		pthread_create (&tids[i], NULL, supporter, &arg_team1) ;
	}
	/* Create the other threads (ie. team2) */
	for ( ; i < nb_threads; i++){
		pthread_create (&tids[i], NULL, supporter, &arg_team2) ;
	}  

	/* Wait until every thread ened */ 
	for (i = 0; i < nb_threads-1; i++){
		pthread_join (tids[i], NULL) ;
	}
  
	free (tids) ;
	
	return EXIT_SUCCESS;
}
