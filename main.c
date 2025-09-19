#include "header.h"

int main(int argc, char const *argv[]);
void *philos_routine(void *arg);
long get_time_ms();
void print_action(t_philo *philos, const char *action);
void *monitor_routine(void *arg);
int all_meals_eaten(t_philo *philos);

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
    pthread_join(monitor, NULL);
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
        pthread_mutex_lock(&philos->rules->meal_info);
        if(philos->rules->is_dead)
        {
            pthread_mutex_unlock(&philos->rules->meal_info);
            return NULL;; // exit cleanly
        }
        pthread_mutex_unlock(&philos->rules->meal_info);
        print_action(philos, "thinking");
        action_eat(philos);
        print_action(philos, "is sleeping");
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
                //*
                // pthread_mutex_lock(&philos->rules->meal_info);
                // long time_since_last_meal = get_time_ms() - philos[i].last_meal;
                // pthread_mutex_lock(&philos->rules->print_mutex);
                // printf("Philosopher %d: %ld ms since last meal\n", philos[i].id, time_since_last_meal);
                // pthread_mutex_unlock(&philos->rules->print_mutex);
                // pthread_mutex_unlock(&philos->rules->meal_info);
                //*/
                print_action(&philos[i], "died");
                philos->rules->who_died = i;
                philos->rules->is_dead = 1;
                pthread_mutex_unlock(&philos->rules->meal_info);
                return (NULL);
            }
        }
        pthread_mutex_unlock(&philos->rules->meal_info);
        if (all_meals_eaten(philos))
        {
            // pthread_mutex_lock(&philos->rules->meal_info);
            // philos->rules->is_dead = 1;
            // pthread_mutex_unlock(&philos->rules->meal_info);
            pthread_mutex_lock(&philos->rules->print_mutex);
            printf("Philosopher HAVES EATEN!!\n");
            pthread_mutex_unlock(&philos->rules->print_mutex);
            return NULL; // stop monitor
        }

        usleep(5000); //1ms
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
    philos->rules->is_dead = 1;
    pthread_mutex_unlock(&philos->rules->meal_info);
    return (1);
}

long get_time_ms()
{
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}


void print_action(t_philo *philos, const char *action)//<timestamp_in_ms> <philosopher_id> <message>
{
    long timestamp;

    pthread_mutex_lock(&philos->rules->meal_info);
    if(philos->rules->is_dead)
    {
        pthread_mutex_unlock(&philos->rules->meal_info);
        //print_action(&philos[philos->rules->who_died], "died");
        return; // exit cleanly
    }
    pthread_mutex_unlock(&philos->rules->meal_info);
    timestamp = (get_time_ms() - philos->rules->start_time);
    pthread_mutex_lock(&philos->rules->print_mutex);
    printf("%ld Philosopher %d %s\n", timestamp, philos->id + 1, action);
    pthread_mutex_unlock(&philos->rules->print_mutex);
}

void safe_usleep(long duration_ms, t_rules *rules)
{
    long slept = 0;
    long interval = 1000; // 1ms

    while (slept < duration_ms * 1000)
    {
        pthread_mutex_lock(&rules->meal_info);
        int dead = rules->is_dead;
        pthread_mutex_unlock(&rules->meal_info);
        if (dead)
            break;
        usleep(interval);
        slept += interval;
    }
}