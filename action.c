#include "header.h"

void action_eat(t_philo *philos)
{
    //pthread_mutex_lock(&philos->rules->print_mutex);
    //printf("Action\n");
    //pthread_mutex_unlock(&philos->rules->print_mutex);
    
    // Acquire both forks before printing or updating anything
    if (philos->id % 2 == 0) 
    {
        pthread_mutex_lock(philos->left_fork);
        print_action(philos, "has taken a fork");
        pthread_mutex_lock(philos->right_fork);
        print_action(philos, "has taken a fork");
    } 
    else 
    {
        pthread_mutex_lock(philos->right_fork);
        print_action(philos, "has taken a fork");
        pthread_mutex_lock(philos->left_fork);
        print_action(philos, "has taken a fork");
    } 

    // CRITICAL SECTION: must print and update last_meal here
    print_action(philos, "is eating");
    pthread_mutex_lock(&philos->rules->meal_info);
    philos->last_meal = get_time_ms();
    philos->meals_eaten += 1;
    pthread_mutex_unlock(&philos->rules->meal_info);

    // Eat
    safe_usleep(philos->rules->time_to_eat, philos->rules);

    // Release both forks after eating
    pthread_mutex_unlock(philos->left_fork);
    pthread_mutex_unlock(philos->right_fork);
}

