#include "header.h"

int     main(int argc, char const *argv[]);
void    *philos_routine(void *arg);
void    *monitor_routine(void *arg);
void    action_eat(t_philo *philos);
int     all_meals_eaten(t_philo *philos);


//number_of_philosophers 
//time_to_die 
//time_to_eat 
//time_to_sleep
//[number_of_times_each_philosopher_must_eat]


int main(int argc, char const *argv[])
{
    t_philo         *philos;
    pthread_t       monitor;

    philos = init_philos(argc, argv);
    for (int i = 0; i < philos->rules->num_philos; i++)
    {
        pthread_create(
            &philos->rules->threads[i], // store thread ID
            NULL,                       // default attributes
            philos_routine,             // function each philosopher runs
            &philos[i]                  // pass philosopher struct as argument
        );
    }
    pthread_create(&monitor, NULL, monitor_routine, philos); // Monitor thread to check for death
    for (int i = 0; i < philos->rules->num_philos; i++)    // Wait for all threads to finish
        pthread_join(philos->rules->threads[i], NULL);
    pthread_join(monitor, NULL);
    cleanup(philos);
    return 0;
}



void *philos_routine(void *arg)
{
    t_philo *philos;

    philos = (t_philo *)arg;
    while(1)
    {
        pthread_mutex_lock(&philos->rules->meal_info);
        if(philos->rules->is_dead)
        {
            pthread_mutex_unlock(&philos->rules->meal_info);
            return NULL;; // exit cleanly
        }
        pthread_mutex_unlock(&philos->rules->meal_info);
        action_print(philos, "thinking");
        action_eat(philos);
        action_print(philos, "is sleeping");
        safe_usleep(philos->rules->time_to_sleep, philos->rules); // Convert ms to us
    }
    return NULL;
}

void *monitor_routine(void *arg)
{
    t_philo *philos;
    int     i;

    philos = (t_philo *)arg;
    while(1)
    {
        pthread_mutex_lock(&philos->rules->meal_info);
        for(i = 0; i < philos->rules->num_philos; i++)
        {
            if(get_time_ms() - philos[i].last_meal > philos[i].rules->time_to_die)
            {
                pthread_mutex_unlock(&philos->rules->meal_info);
                action_print(&philos[i], "died");
                philos->rules->is_dead = 1;
                return (NULL);
            }
        }
        pthread_mutex_unlock(&philos->rules->meal_info);
        if (all_meals_eaten(philos))
        {
            pthread_mutex_lock(&philos->rules->print_mutex);
            printf("All Philosophers have eaten %i times!\n", philos->rules->num_must_eat);
            pthread_mutex_unlock(&philos->rules->print_mutex);
            return (NULL); // stop monitor
        }
    }
    return NULL;
}


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
    take_forks(philos);
    action_print(philos, "is eating");
    pthread_mutex_lock(&philos->rules->meal_info);
    philos->last_meal = get_time_ms();
    philos->meals_eaten += 1;
    pthread_mutex_unlock(&philos->rules->meal_info);
    safe_usleep(philos->rules->time_to_eat, philos->rules); // Eat
    pthread_mutex_unlock(philos->left_fork);
    pthread_mutex_unlock(philos->right_fork);
}


int all_meals_eaten(t_philo *philos)
{
    int i;

    if(philos->rules->num_must_eat == -1)
        return 0;

    pthread_mutex_lock(&philos->rules->meal_info);
    for(i = 0; i < philos->rules->num_philos; i++)
    {
        if(philos[i].meals_eaten < philos->rules->num_must_eat)
        {
            pthread_mutex_unlock(&philos->rules->meal_info);
            return (0);
        }
    }
    philos->rules->is_dead = 1;
    pthread_mutex_unlock(&philos->rules->meal_info);
    return (1);
}

