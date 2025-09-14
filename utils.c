#include "header.h"

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

t_rules *init_rules(int argc, const char *argv[])
{
    t_rules *rules;

    if (argc < 5 || argc > 6)
		clean_exitf(printf("Error: invalid number of arguments\n"), NULL);
	rules = malloc(sizeof(t_rules));
	if (!rules)
		clean_exitf("Error: Memory allocation failed for rules\n", NULL);
    rules->num_philos = ft_atoi(argv[1]);
    rules->time_to_die = ft_atoi(argv[2]);
    rules->time_to_eat = ft_atoi(argv[3]);
    rules->time_to_sleep = ft_atoi(argv[4]);
    if (argc == 6)
        rules->number_of_times_each_must_eat = ft_atoi(argv[5]);
    else
        rules->number_of_times_each_must_eat = -1; // Optional parameter not provided
	if (rules->num_philos <= 0 || rules->time_to_die <= 0
        || rules->time_to_eat <= 0 || rules->time_to_sleep <= 0)
	{
		free(rules);
		clean_exitf(printf("Error: invalid arguments\n"), NULL);
	}
	return (rules);
}

void clean_exitf(char *message, t_philo *philos)
{
	printf("%s", message);
	if (philos)
	{
		if(philos->rules)
		{
			if(philos->rules->forks)
				free(philos->rules->forks);
			free(philos->rules);

		}
		free(philos);
	}
	exit(EXIT_FAILURE);
}

t_philo *init_philos(int argc, const char *argv[]) //It doesn’t start the simulation yet, so it doesn’t create threads.
{
	t_philo			*philos;
	t_rules			*rules; 
	int				i;

	rules = init_rules(argc, argv);
	init_forks(rules->num_philos, rules);

	philos = malloc(sizeof(t_philo) * rules->num_philos);
	if (!philos)
		clean_exitf("Error: Memory allocation failed for philosophers\n", NULL);

	for (i = 0; i < rules->num_philos; i++)
	{
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = 0; // Initialize with appropriate timestamp
		philos[i].left_fork = &rules->forks[i]; //address of the fork/mutex to the left
		philos[i].right_fork = &rules->forks[(i + 1) % rules->num_philos]; //address of the fork to the right //The modulo % wraps the index around when you reach the last philosopher.
		philos[i].rules = rules;
	}
	return (philos);
}

void init_forks_threads_mutex(int num_philosophers, t_rules *rules)
{
	int				i;

	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->num_philos); //one mutex for each fork.
	if (!rules->forks)
	{
		free(rules);
		clean_exitf("Error: Memory allocation failed for forks\n", NULL);
	}
	for (i = 0; i < rules->num_philos; i++)
	{
		pthread_mutex_init(&(rules->forks)[i], NULL);
	}
	pthread_mutex_init(&rules->print, NULL);
	rules->threads = malloc(sizeof(pthread_t) * rules->num_philos);
	if (!rules->threads)
    {
        for (i = 0; i < rules->num_philos; i++)
            pthread_mutex_destroy(&rules->forks[i]);
        free(rules->forks);
        free(rules);
        clean_exitf("Error: Memory allocation failed for threads\n", NULL);
    }

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