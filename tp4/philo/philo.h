#ifndef PHILO_H
#define PHILO_H

#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THINKING 1
#define HUNGRY 2
#define EATING 3

void init(int nb_philos, int *states);
void take_chopstick(int nb_philo);
void put_chopstick(int nb_philo);
void dinner_terminate(void);

#endif
