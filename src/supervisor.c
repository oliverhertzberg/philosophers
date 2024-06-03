/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 01:10:11 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/06/04 01:10:32 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"


int is_dead_or_full(t_monitor *m)
{
    int i;

    i = -1;
    while (++i < m->philo_amt)
    {
        pthread_mutex_lock(&m->philos[i].meal_lock);
        if (m->philos[i].last_meal > m->ms_to_die)
        {
            pthread_mutex_unlock(&m->philos[i].meal_lock);
            pthread_mutex_lock(&m->death_lock);
            m->death = 1;
            pthread_mutex_unlock(&m->death_lock);
            return (write_state(&m->philos[i], "died"), 1);
        }
        else if (m->meals_to_eat && m->philos[i].meals_eaten >= m->meals_to_eat)
        {
            pthread_mutex_unlock(&m->philos[i].meal_lock);
            pthread_mutex_lock(&m->death_lock);
            m->death = 1;
            pthread_mutex_unlock(&m->death_lock);
            return (write_state(&m->philos[i], "ate too much"), 1);
        }
        pthread_mutex_unlock(&m->philos[i].meal_lock);
    }
    return (0);
}

void    supervise(t_monitor *m)
{   
    while(1)
    {
        if (is_dead_or_full(m))
            break ;
    }
}
