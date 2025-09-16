#include "header.h"

void action_eat(t_philo *philos)
{
    if(philos->id % 2 == 0)
    {
        //Take right fork (lock right mutex).
        pthread_mutex_lock(philos->right_fork);
        print_action(philos, "has taken a fork");
    }
    //Take left fork (lock left mutex).
    pthread_mutex_lock(philos->left_fork);
    print_action(philos, "has taken a fork");
    if(philos->id % 2 != 0)
    {
        //Take right fork (lock right mutex).
        pthread_mutex_lock(philos->right_fork);
        print_action(philos, "has taken a fork");
    }


    //print that the philosopher is eating.
    print_action(philos, "is eating");

    //Update last meal time and increment meals eaten.
    pthread_mutex_lock(&philos->rules->meal_info);
    philos->last_meal = get_time_ms();
    philos->meals_eaten += 1;
    pthread_mutex_unlock(&philos->rules->meal_info);

    //Eat for time_to_eat milliseconds.
    usleep(philos->rules->time_to_eat * 1000); // Convert ms to us
    pthread_mutex_unlock(philos->left_fork);
    pthread_mutex_unlock(philos->right_fork);
}
