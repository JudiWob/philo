#ifndef HEADER_H
#define HEADER_H


#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_rules
{
    int num_philos;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_times_each_must_eat; // optional (set -1 if not provided)
    long start_time; // timestamp when simulation starts
    pthread_mutex_t *forks; // array of mutexes for forks
    pthread_mutex_t print; // mutex for printing to avoid jumbled output
    pthread_t *threads;
}   t_rules;

typedef struct s_philo
{
    int id;
    int meals_eaten;
    long last_meal;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_rules *rules; // pointer to global rules
}   t_philo;


int     ft_atoi(const char *nptr);
t_rules *init_rules(int argc, const char *argv[]);
void init_forks_threads_mutex(int num_philosophers, t_rules *rules);
t_philo *init_philos(int argc, const char *argv[]);

#endif