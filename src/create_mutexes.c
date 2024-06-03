/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_mutexes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 01:13:37 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/06/04 01:13:48 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int init_meal_mutex(t_monitor *m)
{
    int i;

    i = -1;
    while (i++ < m->philo_amt)
    {
        if (pthread_mutex_init(&(m->philos[i].meal_lock), NULL) != 0)
        {
            while (--i >= 0)
                pthread_mutex_destroy(&(m->philos[i].meal_lock));
            write(2, "failed to initialize mutex\n", 28);
            return (0);
        }
    }
    return (1);
}

int init_fork_mutex(t_monitor *m)
{
    int i;

    i = -1;
    while (++i < m->philo_amt)
    {
        if (pthread_mutex_init(&m->forks[i], NULL) != 0)
        {
            while (--i >= 0)
                pthread_mutex_destroy(&m->forks[i]);
            write(2, "failed to initialize mutex\n", 28);
            return (0);
        }
    }
    return (1);
}

int create_mutexes(t_monitor *m)
{
    if (!init_meal_mutex(m))
        return (0);
    if (!init_fork_mutex(m))
    {
        destroy_meal_mutex(m);
        return (0);
    }
    if (pthread_mutex_init(&m->write_lock, NULL) != 0)
    {
        destroy_meal_mutex(m);
        destroy_fork_mutex(m);
        write(2, "failed to initialize mutex\n", 28);
        return (0);
    }
    if (pthread_mutex_init(&m->death_lock, NULL) != 0)
    {
        destroy_meal_mutex(m);
        destroy_fork_mutex(m);
        pthread_mutex_destroy(&m->write_lock);
        write(2, "failed to initialize mutex\n", 28);
        return (0);
    }
    return (1);
}
