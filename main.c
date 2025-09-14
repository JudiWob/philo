#include "header.h"

int main(int argc, char const *argv[])
{
    t_philo *philos;

    philos = init_philos(argc, argv);

    (void)(philos);
    // for (int i = 0; i < philos->rules->num_philos; i++)
    // {
    //     pthread_create(
    //         &philos->rules->threads[i], // store thread ID
    //         NULL,                       // default attributes
    //         philos_routine,             // function each philosopher runs
    //         &philos[i]                  // pass philosopher struct as argument
    //     );
    // }

    // // Wait for all threads to finish
    // for (int i = 0; i < philos->rules->num_philos; i++)
    //     pthread_join(philos->rules->threads[i], NULL);

    // // Cleanup
    // for (int i = 0; i < philos->rules->num_philos; i++)
    //     pthread_mutex_destroy(&philos->rules->forks[i]);
    return 0;
}

// void philos_routine()
// {

// }