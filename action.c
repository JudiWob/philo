#include "header.h"

void    safe_usleep(long duration_ms, t_rules *rules);
long    get_time_ms();
void    take_forks(t_philo *philos);
void    action_print(t_philo *philos, const char *action);
void    cleanup(t_philo *philos);

void safe_usleep(long duration_ms, t_rules *rules)
{
    long slept;
    long interval;
    
    interval = 1000; // 1ms
    slept = 0;
    while (slept < duration_ms * 1000)
    {
        pthread_mutex_lock(&rules->meal_info);
        int dead = rules->is_dead;
        pthread_mutex_unlock(&rules->meal_info);
        if (dead)
            break;
        usleep(interval);
        slept += interval;
    }
}

long get_time_ms()
{
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void take_forks(t_philo *philos)
{
    if (philos->id % 2 == 0) 
    {
        pthread_mutex_lock(philos->left_fork);
        action_print(philos, "has taken a fork");
        pthread_mutex_lock(philos->right_fork);
        action_print(philos, "has taken a fork");
    } 
    else 
    {
        pthread_mutex_lock(philos->right_fork);
        action_print(philos, "has taken a fork");
        pthread_mutex_lock(philos->left_fork);
        action_print(philos, "has taken a fork");
    } 
}

void action_print(t_philo *philos, const char *action)//<timestamp_in_ms> <philosopher_id> <message>
{
    long timestamp;

    pthread_mutex_lock(&philos->rules->meal_info);
    if(philos->rules->is_dead)
    {
        pthread_mutex_unlock(&philos->rules->meal_info);
        return;
    }
    pthread_mutex_unlock(&philos->rules->meal_info);
    timestamp = (get_time_ms() - philos->rules->start_time);
    pthread_mutex_lock(&philos->rules->print_mutex);
    printf("%ld Philosopher %d %s\n", timestamp, philos->id + 1, action);
    pthread_mutex_unlock(&philos->rules->print_mutex);
}

void cleanup(t_philo *philos)
{
    int i;
    
    for (i = 0; i < philos->rules->num_philos; i++)
        pthread_mutex_destroy(&philos->rules->forks[i]);
    pthread_mutex_destroy(&philos->rules->print_mutex);
    pthread_mutex_destroy(&philos->rules->meal_info);
    free(philos->rules->threads);
    free(philos->rules->forks);
    free(philos->rules);
    free(philos);
}
