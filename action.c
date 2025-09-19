#include "header.h"


void action_eat(t_philo *philos)
{
    pthread_mutex_lock(&philos->rules->print_mutex);
    printf("Action\n");
    pthread_mutex_unlock(&philos->rules->print_mutex);
    
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
    print_action(philos, "READY eating");
    // Release both forks after eating
    pthread_mutex_unlock(philos->left_fork);
    pthread_mutex_unlock(philos->right_fork);
}


// void action_eat(t_philo *philos)
// {
    
//     pthread_mutex_lock(&philos->rules->meal_info);
//     if(philos->rules->is_dead)
//     {
//         pthread_mutex_unlock(&philos->rules->meal_info);
//         //print_action(&philos[philos->rules->who_died], "died");
//         return; // exit cleanly
//     }
//     pthread_mutex_unlock(&philos->rules->meal_info);
//     if (philos->id % 2 == 0) 
//     {
//         pthread_mutex_lock(philos->left_fork);
//         print_action(philos, "has taken a fork");
//         pthread_mutex_lock(philos->right_fork);
//         print_action(philos, "has taken a fork");
//     } 
//     else 
//     {
//         pthread_mutex_lock(philos->right_fork);
//         print_action(philos, "has taken a fork");
//         pthread_mutex_lock(philos->left_fork);
//         print_action(philos, "has taken a fork");
//     } 

//     //print that the philosopher is eating.
//     print_action(philos, "is eating");

//     //Update last meal time and increment meals eaten.
//     pthread_mutex_lock(&philos->rules->meal_info);
//     philos->last_meal = get_time_ms();
//     philos->meals_eaten += 1;
//     pthread_mutex_unlock(&philos->rules->meal_info);

//     //Eat for time_to_eat milliseconds.
//     safe_usleep(philos->rules->time_to_eat * 1000, philos->rules); // Convert ms to us
//     pthread_mutex_unlock(philos->left_fork);
//     pthread_mutex_unlock(philos->right_fork);

// }

    
    
    // if(philos->id % 2 == 0)
    // {
    //     //Take right fork (lock right mutex).
    //     pthread_mutex_lock(philos->right_fork);
        
    // }
    // //Take left fork (lock left mutex).
    // pthread_mutex_lock(philos->left_fork);
    // print_action(philos, "has taken a fork");
    // if(philos->id % 2 != 0)
    // {
    //     //Take right fork (lock right mutex).
    //     pthread_mutex_lock(philos->right_fork);
    //     print_action(philos, "has taken a fork");
    // }