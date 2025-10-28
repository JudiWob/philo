/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaselt <jpaselt@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 16:58:26 by jpaselt           #+#    #+#             */
/*   Updated: 2025/10/27 16:58:28 by jpaselt          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	*philos_routine(void *arg);
void	action_eat(t_philo *philos);
void	take_forks(t_philo *philos);
void	action_print(t_philo *philos, const char *action);


void	*philos_routine(void *arg)
{
	t_philo	*philos;

	philos = (t_philo *)arg;
	pthread_mutex_lock(&philos->check_mutex);
	philos->last_meal = philos->rules->start_time;
	pthread_mutex_unlock(&philos->check_mutex);
	action_print(philos, "thinking");
	while (alive(philos))
	{
		action_eat(philos);
		action_print(philos, "is sleeping");
		safe_usleep(philos->rules->time_to_sleep, philos);
		action_print(philos, "thinking");
	}
	return (NULL);
}


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