/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaselt <jpaselt@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:36:43 by jpaselt           #+#    #+#             */
/*   Updated: 2025/10/27 16:57:50 by jpaselt          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_rules
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_must_eat;
	long			start_time;
	pthread_t		*threads;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
}					t_rules;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal;
	int				is_dead;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	check_mutex;
	t_rules			*rules;
}					t_philo;

// main
int					main(int argc, char const *argv[]);
void				*philos_routine(void *arg);
void				*monitor_routine(void *arg);
void				action_eat(t_philo *philos);
int					all_meals_eaten(t_philo *philos);

// utils 2
void				safe_usleep(long duration_ms, t_philo *philos);
long				get_time_ms(void);
void				take_forks(t_philo *philos);
void				action_print(t_philo *philos, const char *action);
int					cleanup(t_philo *philos);

// utils 2
t_rules				*init_rules(int argc, const char *argv[]);
t_philo				*init_philos(int argc, const char *argv[]);
void				init_forks_threads_mutex(t_rules *rules);
int					ft_atoi(const char *nptr);
void				clean_exitf(char *message, t_rules *rules);

int					alive(t_philo *philos);
void				ft_sleep(size_t sleep);

#endif