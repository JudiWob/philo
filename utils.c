#include "header.h"

t_rules	*init_rules(int argc, const char *argv[]);
t_philo	*init_philos(int argc, const char *argv[]);
void	init_forks_threads_mutex(t_rules *rules);
int		ft_atoi(const char *nptr);
void	clean_exitf(char *message, t_rules *rules);

t_philo	*init_philos(int argc, const char *argv[])
{
	t_philo			*philos;
	t_rules			*rules;
	int				i;

	i = 0;
	if (argc < 5)
		clean_exitf("Invalid arguments\n", NULL);
	rules = init_rules(argc, argv);
	init_forks_threads_mutex(rules);
	philos = malloc(sizeof(t_philo) * rules->num_philos);
	if (!philos)
		exit(EXIT_FAILURE);
	while (i < rules->num_philos)
	{
		philos[i].id = i;
		philos[i].meals_eaten = 0;
		philos[i].is_dead = 0;
		philos[i].left_fork = &rules->forks[i];
		philos[i].right_fork = &rules->forks[(i + 1) % rules->num_philos];
		pthread_mutex_init(&philos[i].check_mutex, NULL);
		philos[i].rules = rules;
		i++;
	}
	return (philos);
}

t_rules	*init_rules(int argc, const char *argv[])
{
	t_rules	*rules;

	if (argc < 5 || argc > 6)
		exit(EXIT_FAILURE);
	rules = malloc(sizeof(t_rules));
	if (!rules)
		exit(EXIT_FAILURE);
	rules->num_philos = ft_atoi(argv[1]);
	rules->time_to_die = ft_atoi(argv[2]);
	rules->time_to_eat = ft_atoi(argv[3]);
	rules->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		rules->num_must_eat = ft_atoi(argv[5]);
	else
		rules->num_must_eat = -1;
	if (rules->num_philos <= 0 || rules->time_to_die <= 0
		|| rules->time_to_eat <= 0 || rules->time_to_sleep <= 0)
		clean_exitf("Invalid arguments\n", rules);
	rules->start_time = get_time_ms();
	return (rules);
}

void	init_forks_threads_mutex(t_rules *rules)
{
	int				i;

	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->num_philos); 
	if (!rules->forks)
		exit(EXIT_FAILURE);
	i = 0;
	while(i < rules->num_philos)
	{
		pthread_mutex_init(&(rules->forks)[i], NULL);
		i++;
	}
	pthread_mutex_init(&rules->print_mutex, NULL);
	rules->threads = malloc(sizeof(pthread_t) * rules->num_philos);
	if (!rules->threads)
		exit(EXIT_FAILURE);
}


void clean_exitf(char *message, t_rules *rules)
{
	printf("Error: %s", message);
	if(rules)
		free(rules);
	exit(EXIT_FAILURE);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = (10 * result) + (nptr[i] - 48);
		i++;
	}
	return (sign * result);
}
