/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_monitor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaselt <jpaselt@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 18:09:20 by jpaselt           #+#    #+#             */
/*   Updated: 2025/10/28 20:37:47 by jpaselt          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	*monitor_routine(void *arg);
int		check_starvation(t_philo *philos, int i);
void	kill_all(t_philo *philos, int id);
int		all_meals_eaten(t_philo *philos);

void	*monitor_routine(void *arg)
{
	int		i;
	t_philo	*philos;

	philos = (t_philo *)arg;
	ft_sleep(philos->rules->time_to_die - 20);
	while (1)
	{
		pthread_mutex_lock(&philos->rules->print_mutex);
		if (all_meals_eaten(philos))
			return (NULL);
		i = 0;
		while (i < philos->rules->num_philos)
		{
			if (check_starvation(philos, i))
				return (NULL);
			i++;
		}
		pthread_mutex_unlock(&philos->rules->print_mutex);
		usleep(200);
	}
	return (NULL);
}

int	check_starvation(t_philo *philos, int i)
{
	pthread_mutex_lock(&philos[i].check_mutex);
	if (get_time_ms()
		- philos[i].last_meal >= philos[i].rules->time_to_die)
	{
		pthread_mutex_unlock(&philos[i].check_mutex);
		kill_all(philos, i);
		return (1);
	}
	pthread_mutex_unlock(&philos[i].check_mutex);
	return (0);
}

void	kill_all(t_philo *philos, int id)
{
	int		i;
	long	timestamp;

	timestamp = (get_time_ms() - philos->rules->start_time);
	i = 0;
	while (i < philos->rules->num_philos)
	{
		pthread_mutex_lock(&philos[i].check_mutex);
		philos[i].is_dead = 1;
		pthread_mutex_unlock(&philos[i].check_mutex);
		i++;
	}
	if (id != -1)
	{
		usleep(500);
		printf("%ld %d died\n", timestamp, id + 1);
	}
	pthread_mutex_unlock(&philos->rules->print_mutex);
}

int	all_meals_eaten(t_philo *philos)
{
	int	i;

	if (philos->rules->num_must_eat == -1)
		return (0);
	i = 0;
	while (i < philos->rules->num_philos)
	{
		pthread_mutex_lock(&philos[i].check_mutex);
		if (philos[i].meals_eaten < philos->rules->num_must_eat)
		{
			pthread_mutex_unlock(&philos[i].check_mutex);
			return (0);
		}
		pthread_mutex_unlock(&philos[i].check_mutex);
		i++;
	}
	kill_all(philos, -1);
	usleep(500);
	printf("All Philosophers have eaten %i times!\n",
		philos->rules->num_must_eat);
	pthread_mutex_unlock(&philos->rules->print_mutex);
	return (1);
}
