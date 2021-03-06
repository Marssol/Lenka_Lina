#ifndef BARMAN_H
#define BARMAN_H

#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Clients
#define THINKING 1
#define SERVED  2
#define DRINKING  3
//Clients and Barmans
#define WAITING 4
//Barmans
#define DISPONIBLE 5
#define BUSY 6
//Bottles
#define UNAVAILABLE 8
#define AVAILABLE 9

//Set bottles for drink client choice
typedef struct Choice_bottles {
	int nb_bottles;
	int *set_bottles; 
} Choice_bottles;

void init(int n_clients, int m_barmans, int k_bottles, int **states_barmans, int *states_bottles, int *states_clients);
void take_barman(int id_client, Choice_bottles str_set_bottles);
int ready(int id_barman);
void take_bottles(int id_barman);
void put_botlles_free_barman(int id_barman);
void bar_close(void);
void go_home();
void finishDrink(int id);

#endif
