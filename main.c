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

int function(t_philo *philos)
{
            pthread_mutex_lock(&philos->rules->meal_info);
        if(philos->rules->is_dead)
        {
            pthread_mutex_unlock(&philos->rules->meal_info);
            return 0; // exit cleanly
        }
        pthread_mutex_unlock(&philos->rules->meal_info);
    return 1;
}

void *philos_routine(void *arg)
{
    t_philo *philos;

    philos = (t_philo *)arg;
    action_print(philos, "thinking");
    if (philos->id % 2 == 0)
        usleep(3000);
    while(function(philos))
    {
        if (philos->id % 2 == 0)
            usleep(500);
        action_eat(philos);
        action_print(philos, "is sleeping");
        usleep(philos->rules->time_to_sleep * 1000);
        action_print(philos, "thinking");
    }
    return NULL;
}

void *monitor_routine(void *arg)
{
    int     i;
    t_philo *philos;

    philos = (t_philo *)arg;
    usleep(philos->rules->time_to_die * 1000);
    while(1)
    {
        if (all_meals_eaten(philos))
            return (NULL); // stop monitor
        pthread_mutex_lock(&philos->rules->meal_info);
        for(i = 0; i < philos->rules->num_philos; i++)
        {
            if(get_time_ms() - philos[i].last_meal > philos[i].rules->time_to_die)
            {
                pthread_mutex_unlock(&philos->rules->meal_info);
                action_print(&philos[i], "died");
                pthread_mutex_lock(&philos->rules->death);
                philos->rules->is_dead = 1;
                pthread_mutex_unlock(&philos->rules->death);
                return (NULL);
            }
        }
        pthread_mutex_unlock(&philos->rules->meal_info);
        usleep(1000);
    }
    return NULL;
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
    pthread_mutex_lock(&philos->rules->death);
    philos->rules->is_dead = 1;
    pthread_mutex_unlock(&philos->rules->death);
    pthread_mutex_lock(&philos->rules->print_mutex);
    printf("All Philosophers have eaten %i times!\n", philos->rules->num_must_eat);
    pthread_mutex_unlock(&philos->rules->print_mutex);
    return (1);
}

