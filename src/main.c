/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:02:38 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/03/22 17:00:17 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/*
Your program should take the following arguments:
argv[1] = number_of_philosophers 
argv[2] = time_to_die (ms)
argv[3] = time_to_eat (ms)
argv[4] = time_to_sleep (ms)
argv[5] = [number_of_times_each_philosopher_must_eat] (OPTIONAL)
*/

void    invalid_arguments(void)
{
    printf("Invalid arguments!\n"
    "Please provide the following arguments:\n"
    "1: number_of_philosophers, 2: time_to_die(ms), 3: time_to_eat(ms), 4: time_to_sleep(ms), "
    "5(OPTIONAL): [number_of_times_each_philosopher_must_eat]\n"
    );
    exit(1);
}

void    error_exit(char *s, int exitcode)
{
    printf("%s\n", s);
    exit (exitcode);    
}

void    init_variables(t_args *a, int argc, char **argv)
{
    a->philo_amt = ft_atoi(argv[1]);
    a->ms_die = ft_atoi(argv[2]);
    a->ms_eat = ft_atoi(argv[3]);
    a->ms_sleep = ft_atoi(argv[4]);
    if (argc == 6)
        a->meal_amt = ft_atoi(argv[5]); 
    else
        a->meal_amt = -1;
}
void    *routine(void *philo)
{
    struct timeval start, end;
    gettimeofday(&start, NULL);
    sleep(3);
    gettimeofday(&end, NULL);
    printf("P%d and time alive = %ldms\n", *(int *)philo, ((end.tv_sec - start.tv_sec) * 1000));
    free (philo);
    return (NULL);
}

void    create_threads(t_args *a)
{
    int *philo;
    int i;
    
    a->philo_t = malloc(sizeof(pthread_t) * a->philo_amt);
    if (!a->philo_t)
        error_exit("malloc failed for philo_t\n", 1);
    i = 0;
    while (i < a->philo_amt)
    {
        philo = malloc(sizeof(int));
        *philo = i + 1;
        pthread_create(&a->philo_t[i++], NULL, &routine, (void *)philo);
    }
    i = 0;
    while (i < a->philo_amt)
        pthread_join(a->philo_t[i++], NULL);
}

void    create_mutex_forks(t_args *a)
{
    int i;
    
    a->mutexes = malloc(sizeof(pthread_mutex_t) * a->philo_amt);
    if (!a->mutexes)
        error_exit("malloc failed for mutexes\n", 1);
    i = 0;
    while (i < a->philo_amt)
        if (pthread_mutex_init(&a->mutexes[i++], NULL) != 0)
            error_exit("failed to initialize mutexes\n", 1);
}

int main(int argc, char **argv)
{
    t_args a;
    
    if (argc < 5 || argc > 6)
        invalid_arguments();
    init_variables(&a, argc, argv);
    create_mutex_forks(&a);
    create_threads(&a);
    free (a.philo_t);
    free (a.mutexes);
    return (0);
}