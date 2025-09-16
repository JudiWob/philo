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
    int num_must_eat; // optional (set -1 if not provided)
    int is_dead;
    long start_time; // timestamp when simulation starts
    pthread_mutex_t *forks; // array of mutexes for forks
    pthread_mutex_t print_mutex; // mutex for printing to avoid jumbled output
    pthread_mutex_t meal_info;   // protects last_meal & meals_eaten
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



void      print_philos(t_philo *philos);
int       ft_atoi(const char *nptr);
void      clean_exitf(char *message, t_philo *philos);
t_rules   *init_rules(int argc, const char *argv[]);
void      init_forks_threads_mutex(t_rules *rules);
t_philo   *init_philos(int argc, const char *argv[]);
void      *philos_routine(void *arg);
void      print_action(t_philo *philos, const char *action);
long      get_time_ms();
void      action_eat(t_philo *philos);
void      *monitor_routine(void *arg);

#endif