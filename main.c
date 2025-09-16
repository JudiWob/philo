#include "header.h"

int main(int argc, char const *argv[]);
void *philos_routine(void *arg);
long get_time_ms();
void print_action(t_philo *philos, const char *action);

int main(int argc, char const *argv[])
{
    t_philo         *philos;
    pthread_t       monitor;

    philos = init_philos(argc, argv);
    print_philos(philos);

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
    // Wait for all threads to finish
    for (int i = 0; i < philos->rules->num_philos; i++)
        pthread_join(philos->rules->threads[i], NULL);
    // Cleanup
    for (int i = 0; i < philos->rules->num_philos; i++)
        pthread_mutex_destroy(&philos->rules->forks[i]);
    return 0;
}

void *philos_routine(void *arg)
{
    t_philo *philos;

    philos = (t_philo *)arg;

    while(1)
    {
        print_action(philos, "thinking");
        action_eat(philos);
        print_action(philos, "is sleeping");
        usleep(philos->rules->time_to_sleep * 1000); // Convert ms to us
    }
    return NULL;
}

void *monitor_routine(void *arg)
{
    t_philo *philos;
    int     i;

    philos = (t_philo *)arg;

    while(!philos->rules->is_dead || )
    {
        for(i = 0; i < philos->rules->num_philos; i++)
        {
            pthread_mutex_lock(&philos->rules->meal_info);
            if(get_time_ms() - philos[i].last_meal > philos[i].rules->time_to_die)
            {
                print_action(&philos[i], "died");
                philos[i].rules->is_dead = 1;
            }
            pthread_mutex_unlock(&philos->rules->meal_info);
        }
        usleep(1000); //1ms
    }
    return NULL;
}
    

long get_time_ms()
{
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}


void print_action(t_philo *philos, const char *action)
{
    //<timestamp_in_ms> <philosopher_id> <message>
    long timestamp;

    timestamp = (get_time_ms() - philos->rules->start_time);
    pthread_mutex_lock(&philos->rules->print_mutex);
    printf("%ld Philosopher %d %s\n", timestamp, philos->id, action);
    pthread_mutex_unlock(&philos->rules->print_mutex);

}