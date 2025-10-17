#include "header.h"

void    safe_usleep(long duration_ms, t_rules *rules);
long    get_time_ms();
void    take_forks(t_philo *philos);
void    action_print(t_philo *philos, const char *action);
void    cleanup(t_philo *philos);

void action_eat(t_philo *philos)
{
    
    if (philos->rules->num_philos == 1)
    {
        pthread_mutex_lock(&philos->rules->forks[0]);
        action_print(philos, "has taken a fork");
        safe_usleep(philos->rules->time_to_die, philos->rules);
        pthread_mutex_unlock(&philos->rules->forks[0]);
        return;  // exit this philosopher thread cleanly
    }
    //action_print(philos, "waiting for forks");
    take_forks(philos);
    pthread_mutex_lock(&philos->rules->meal_info);
    philos->last_meal = get_time_ms();
    philos->meals_eaten += 1;
    pthread_mutex_unlock(&philos->rules->meal_info);
    action_print(philos, "is eating");
    usleep(philos->rules->time_to_eat * 1000);
    //safe_usleep(philos->rules->time_to_eat, philos->rules); // Eat
    //action_print(philos, "has finished eating");
    pthread_mutex_unlock(philos->left_fork);
    pthread_mutex_unlock(philos->right_fork);
    
}

void safe_usleep(long duration_ms, t_rules *rules)
{
    long slept;
    long interval;
    int dead;

    interval = 1000; // 1ms
    slept = 0;
    while (slept < duration_ms * 1000)
    {
        pthread_mutex_lock(&rules->meal_info);
        dead = rules->is_dead;
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

    pthread_mutex_lock(&philos->rules->death);
    if(philos->rules->is_dead)
    {
        pthread_mutex_unlock(&philos->rules->death);
        return;
    }
    pthread_mutex_unlock(&philos->rules->death);
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
