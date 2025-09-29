#include "header.h"

void    print_philos(t_philo *philos)
{
    int n = philos->rules->num_philos;
        
    for (int i = 0; i < n; i++)
    {
    printf("Philo %d:\n", philos[i].id);
    printf("  meals_eaten: %d\n", philos[i].meals_eaten);
    printf("  last_meal:   %ld\n", philos[i].last_meal);
    printf("  left_fork:   %p\n", (void*)philos[i].left_fork);
    printf("  right_fork:  %p\n", (void*)philos[i].right_fork);
    printf("  rules ptr:   %p\n", (void*)philos[i].rules);
    printf("-----------------------------\n");
    }

    t_rules *rules = philos->rules;
    printf("=== Simulation Rules ===\n");
    printf("Number of philosophers: %d\n", rules->num_philos);
    printf("Time to die: %d ms\n", rules->time_to_die);
    printf("Time to eat: %d ms\n", rules->time_to_eat);
    printf("Time to sleep: %d ms\n", rules->time_to_sleep);
    printf("Number of meals required: %d\n", rules->num_must_eat);
    printf("Simulation is_dead flag: %d\n", rules->is_dead);
    printf("Start time: %ld\n", rules->start_time);
}