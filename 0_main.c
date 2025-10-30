/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaselt <jpaselt@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 16:50:01 by jpaselt           #+#    #+#             */
/*   Updated: 2025/10/30 16:17:52 by jpaselt          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

// number_of_philosophers
// time_to_die
// time_to_eat
// time_to_sleep
// [number_of_times_each_philosopher_must_eat]

int	main(int argc, char const *argv[])
{
	t_philo		*philos;
	pthread_t	monitor;
	int			i;

	philos = init_philos(argc, argv);
	i = -1;
	while (++i < philos->rules->num_philos)
		pthread_create(&philos->rules->threads[i], NULL, philos_routine,
			&philos[i]);
	pthread_create(&monitor, NULL, monitor_routine, philos);
	i = -1;
	while (++i < philos->rules->num_philos)
		pthread_join(philos->rules->threads[i], NULL);
	pthread_join(monitor, NULL);
	cleanup(philos);
	return (0);
}
