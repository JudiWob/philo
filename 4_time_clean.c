/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4_time_clean.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaselt <jpaselt@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 16:48:01 by jpaselt           #+#    #+#             */
/*   Updated: 2025/10/28 19:26:26 by jpaselt          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	ft_sleep(size_t sleep);
void	safe_usleep(long duration_ms, t_philo *rules);
long	get_time_ms(void);
int		cleanup(t_philo *philos);

void	ft_sleep(size_t sleep)
{
	time_t	t;

	t = get_time_ms() + sleep;
	while (get_time_ms() < t)
		usleep(500);
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
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	cleanup(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->rules->num_philos)
	{
		pthread_mutex_destroy(&philos->rules->forks[i]);
		pthread_mutex_destroy(&philos[i].check_mutex);
		i++;
	}
	pthread_mutex_destroy(&philos->rules->print_mutex);
	free(philos->rules->threads);
	free(philos->rules->forks);
	free(philos->rules);
	free(philos);
	return (0);
}
