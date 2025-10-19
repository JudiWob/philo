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
    int done;
    long start_time; // timestamp when simulation starts
    pthread_mutex_t *forks; // array of mutexes for forks
    pthread_mutex_t print_mutex; // mutex for printing to avoid jumbled output
    pthread_mutex_t death;
    pthread_mutex_t general;
    pthread_t *threads;
}   t_rules;

typedef struct s_philo
{
    int id;
    int meals_eaten;
    long last_meal;
    int is_dead;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_rules *rules; // pointer to global rules
    pthread_mutex_t check_mutex;
}   t_philo;

//main
int     main(int argc, char const *argv[]);
void    *philos_routine(void *arg);
void    *monitor_routine(void *arg);
void    action_eat(t_philo *philos);
int     all_meals_eaten(t_philo *philos);

//utils 2
void    safe_usleep(long duration_ms, t_philo *philos);
long    get_time_ms();
void    take_forks(t_philo *philos);
void    action_print(t_philo *philos, const char *action);
void    cleanup(t_philo *philos);

//utils 2
t_rules	*init_rules(int argc, const char *argv[]);
t_philo	*init_philos(int argc, const char *argv[]);
void 	init_forks_threads_mutex(t_rules *rules);
int		ft_atoi(const char *nptr);
void 	clean_exitf(char *message, t_rules *rules);

int alive(t_philo *philos);


void    print_philos(t_philo *philos);

#endif