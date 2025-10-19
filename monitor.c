#include   "header.h"

void kill_all(t_philo *philos, int id)
{
    int i;
    long timestamp;

    timestamp = (get_time_ms() - philos->rules->start_time);
    if(id != -1)
    {
        pthread_mutex_lock(&philos->rules->print_mutex);
        printf("%ld Philosopher %d died\n", timestamp, id + 1);
        pthread_mutex_unlock(&philos->rules->print_mutex);
    }
    for (i = 0; i < philos->rules->num_philos; i++)
    {
        pthread_mutex_lock(&philos[i].check_mutex);
        philos[i].is_dead = 1;
        pthread_mutex_unlock(&philos[i].check_mutex);
    }
}


int all_meals_eaten(t_philo *philos)
{
    int i;

    if(philos->rules->num_must_eat == -1)
        return 0;
    for(i = 0; i < philos->rules->num_philos; i++)
    {
        pthread_mutex_lock(&philos[i].check_mutex);
        if(philos[i].meals_eaten < philos->rules->num_must_eat)
        {
            pthread_mutex_unlock(&philos[i].check_mutex);
            return (0);
        }
        pthread_mutex_unlock(&philos[i].check_mutex);
    }
    pthread_mutex_lock(&philos->rules->general);
    philos->rules->done = 1;
    pthread_mutex_unlock(&philos->rules->general);
    kill_all(philos, -1);
    pthread_mutex_lock(&philos->rules->print_mutex);
    printf("All Philosophers have eaten %i times!\n", philos->rules->num_must_eat);
    pthread_mutex_unlock(&philos->rules->print_mutex);
    return (1);
}

void *monitor_routine(void *arg)
{
    int     i;
    t_philo *philos;

    philos = (t_philo *)arg;
    while(1)
    {
        if (all_meals_eaten(philos))
            return (NULL); // stop monitor
        for(i = 0; i < philos->rules->num_philos; i++)
        {
            pthread_mutex_lock(&philos[i].check_mutex);
            if(get_time_ms() - philos[i].last_meal >= philos[i].rules->time_to_die)
            {
                pthread_mutex_unlock(&philos[i].check_mutex);
                kill_all(philos, i);
                return (NULL);
            }
            pthread_mutex_unlock(&philos[i].check_mutex);
        }
        //usleep(500);
    }
    return NULL;
}