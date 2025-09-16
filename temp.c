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
}