#include "header.h"


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
