#include "header.h"

t_rules	*init_rules(int argc, const char *argv[]);
t_philo	*init_philos(int argc, const char *argv[]);
void 	init_forks_threads_mutex(t_rules *rules);
int		ft_atoi(const char *nptr);
void 	clean_exitf(char *message, t_rules *rules);

t_philo *init_philos(int argc, const char *argv[]) //It doesn’t start the simulation yet, so it doesn’t create threads.
{
	t_philo			*philos;
	t_rules			*rules; 
	int				i;

	if (argc < 5)
		clean_exitf("Invalid arguments\n", NULL);
	rules = init_rules(argc, argv);
	init_forks_threads_mutex(rules);
	philos = malloc(sizeof(t_philo) * rules->num_philos);
	if (!philos)
		exit(EXIT_FAILURE);
	for (i = 0; i < rules->num_philos; i++)
	{
		philos[i].id = i;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = get_time_ms(); // Initialize with appropriate timestamp
		philos[i].left_fork = &rules->forks[i]; //address of the fork/mutex to the left
		philos[i].right_fork = &rules->forks[(i + 1) % rules->num_philos]; //address of the fork to the right //The modulo % wraps the index around when you reach the last philosopher.
		philos[i].rules = rules;
	}
	return (philos);
}

t_rules *init_rules(int argc, const char *argv[])
{
	t_rules *rules;
	
    if (argc < 5 || argc > 6)
		exit(EXIT_FAILURE);
	rules = malloc(sizeof(t_rules));
	if (!rules)
		exit(EXIT_FAILURE);
    rules->num_philos = ft_atoi(argv[1]);
    rules->time_to_die = ft_atoi(argv[2]);
    rules->time_to_eat = ft_atoi(argv[3]);
    rules->time_to_sleep = ft_atoi(argv[4]);
	rules->is_dead = 0;
    if (argc == 6)
		rules->num_must_eat = ft_atoi(argv[5]);
    else
		rules->num_must_eat = -1; // Optional parameter not provided
	if (rules->num_philos <= 0 || rules->time_to_die <= 0
        || rules->time_to_eat <= 0 || rules->time_to_sleep <= 0)
		clean_exitf("Invalid arguments\n", rules);
	rules->start_time = get_time_ms(); // Will be set when simulation starts
	return (rules);
}

void init_forks_threads_mutex(t_rules *rules)
{
	int				i;
	
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->num_philos); //one mutex for each fork.
	if (!rules->forks)
		exit(EXIT_FAILURE);
	for (i = 0; i < rules->num_philos; i++)
	{
		pthread_mutex_init(&(rules->forks)[i], NULL);
	}
	pthread_mutex_init(&rules->print_mutex, NULL);
	pthread_mutex_init(&rules->meal_info, NULL);
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
	
	
	
// In the problem: philosophers must not pick up the same fork at the same time.

// In C: a mutex is the tool that guarantees only one thread can lock it at a time

/*
Philosopher i	Left Fork	Right Fork
	0			forks[0]	forks[1]
	1			forks[1]	forks[2]
	2			forks[2]	forks[3]
	3			forks[3]	forks[4]
	4			forks[4]	forks[0] ← wraps around
*/