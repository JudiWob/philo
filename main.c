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
    if (philos->rules->num_philos == 1)
    {
        printf("0 Philosopher 1 died\n");
        cleanup(philos);
        return 0;  // exit this philosopher thread cleanly
    }
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
    // for (int i = 0; i < philos->rules->num_philos; i++)
    // {
    //     printf("They ate %i times\n", philos[i].meals_eaten);
    // }
    cleanup(philos);
    return 0;
}

int alive(t_philo *philos)
{
    pthread_mutex_lock(&philos->check_mutex);
    if(philos->is_dead)
    {
        pthread_mutex_unlock(&philos->check_mutex);
        return 0;
    }
    pthread_mutex_unlock(&philos->check_mutex);
    return 1;
}

void	ft_sleep(size_t sleep)
{
	time_t	t;

	t = get_time_ms() + sleep;
	while (get_time_ms() < t)
		usleep(500);
}

void *philos_routine(void *arg)
{
    t_philo *philos;

    philos = (t_philo *)arg;
    pthread_mutex_lock(&philos->check_mutex);
    philos->last_meal = philos->rules->start_time;
    pthread_mutex_unlock(&philos->check_mutex);
    action_print(philos, "thinking");
    if (philos->id % 2 == 0)
        ft_sleep(philos->rules->time_to_eat);
        //usleep(1000);
    while(alive(philos))
    {
        if (philos->id % 2 == 0)
            usleep(500);
        action_eat(philos);
        action_print(philos, "is sleeping");
        safe_usleep(philos->rules->time_to_sleep, philos);
        action_print(philos, "thinking");
    }
    return NULL;
}


