#include "header.h"

void	safe_usleep(long duration_ms, t_philo *rules);
long	get_time_ms(void);
void	take_forks(t_philo *philos);
void	action_print(t_philo *philos, const char *action);
int		cleanup(t_philo *philos);

void	action_eat(t_philo *philos)
{
	take_forks(philos);
	pthread_mutex_lock(&philos->check_mutex);
	if (get_time_ms() - philos->last_meal >= philos->rules->time_to_die)
	{
		pthread_mutex_unlock(philos->left_fork);
		pthread_mutex_unlock(philos->right_fork);
		pthread_mutex_unlock(&philos->check_mutex);
		return ;
	}
	philos->last_meal = get_time_ms();
	philos->meals_eaten += 1;
	pthread_mutex_unlock(&philos->check_mutex);
	action_print(philos, "is eating");
	safe_usleep(philos->rules->time_to_eat, philos);
	pthread_mutex_unlock(philos->left_fork);
	pthread_mutex_unlock(philos->right_fork);
}

void	safe_usleep(long sleeptime_ms, t_philo *philos)
{
	long	start_time;

	start_time = get_time_ms();
	while (alive(philos))
	{
		if ((get_time_ms() - start_time) >= sleeptime_ms)
			return ;
		usleep(300);
	}
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + ((tv.tv_usec) / 1000);
}

void	take_forks(t_philo *philos)
{
	if (philos->id % 2 != 0)
		usleep(1000);
	if (philos->id == philos->rules->num_philos)
		usleep(1000);
	if (philos->id % 2 == 0)
	{
		pthread_mutex_lock(philos->left_fork);
		action_print(philos, "has taken a fork");
		pthread_mutex_lock(philos->right_fork);
	}
	else
	{
		pthread_mutex_lock(philos->right_fork);
		action_print(philos, "has taken a fork");
		pthread_mutex_lock(philos->left_fork);
	}
	action_print(philos, "has taken a fork");
}

void	action_print(t_philo *philos, const char *action)
{
	long	timestamp;

	if (!alive(philos))
		return ;
	timestamp = (get_time_ms() - philos->rules->start_time);
	pthread_mutex_lock(&philos->rules->print_mutex);
	printf("%ld %d %s\n", timestamp, philos->id + 1, action);
	pthread_mutex_unlock(&philos->rules->print_mutex);
}

int	cleanup(t_philo *philos)
{
	int	i;

	for (i = 0; i < philos->rules->num_philos; i++)
	{
		pthread_mutex_destroy(&philos->rules->forks[i]);
		pthread_mutex_destroy(&philos[i].check_mutex);
	}
	pthread_mutex_destroy(&philos->rules->print_mutex);
	free(philos->rules->threads);
	free(philos->rules->forks);
	free(philos->rules);
	free(philos);
	return (0);
}
