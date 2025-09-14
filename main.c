#include "header.h"

int main(int argc, char const *argv[])
{
    t_philo         *philos;

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
            
    // Wait for all threads to finish
    for (int i = 0; i < philos->rules->num_philos; i++)
        pthread_join(philos->rules->threads[i], NULL);
    // Cleanup
    for (int i = 0; i < philos->rules->num_philos; i++)
        pthread_mutex_destroy(&philos->rules->forks[i]);
    return 0;
}

void philos_routine(t_philo *philos)
{
    //eat, sleep, think
    print_action(philos, "is thinking");

}

long get_time_ms()
{
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}


void    print_philos(t_philo *philos)
{
    int n = philos->rules->num_philos;
        
    for (int i = 0; i < n; i++)
    {
    printf("Philo %d:\n", philos[i].id);
    printf("  meals_eaten: %d\n", philos[i].meals_eaten);
    printf("  last_meal:   %ld\n", philos[i].last_meal);
    printf("  left_fork:   %p\n", (void*)philos[i].left_fork);
    printf("  right_fork:  %p\n", (void*)philos[i].right_fork);
    printf("  rules ptr:   %p\n", (void*)philos[i].rules);
    printf("-----------------------------\n");
    }
}

void print_action(t_philo *philos, const char *action)
{
    //<timestamp_in_ms> <philosopher_id> <message>
    long timestamp;

    timestamp = (get_time_ms() - philos->rules->start_time);
    pthread_mutex_lock(&philos->rules->print_mutex);
    printf("%ld %d %s\n", timestamp, philos->id, action);
    pthread_mutex_unlock(&philos->rules->print_mutex);
}