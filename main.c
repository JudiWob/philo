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

int	main(int argc, char const *argv[])
{
	t_philo		*philos;
	pthread_t	monitor;
	int			i;

	philos = init_philos(argc, argv);
	if (philos->rules->num_philos == 1)
		return (printf("0 1 died\n"), cleanup(philos));
	i = -1;
	while (++i < philos->rules->num_philos)
		pthread_create(&philos->rules->threads[i], NULL, philos_routine,
			&philos[i]);
	pthread_create(&monitor, NULL, monitor_routine, philos);
	i = -1;
	while (++i < philos->rules->num_philos)
		pthread_join(philos->rules->threads[i], NULL);
	pthread_join(monitor, NULL);
	return (cleanup(philos));
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


