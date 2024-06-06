/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 01:10:11 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/06/06 18:17:04 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	are_all_done_eating_or_dead(t_monitor *m)
{
	int	i;

	i = -1;
	while (++i < m->philo_amt)
	{
		pthread_mutex_lock(&m->philos[i].meal_lock);
		if (m->philos[i].eaten_enough == 1)
		{
			pthread_mutex_unlock(&m->philos[i].meal_lock);
			return (1);
		}
		pthread_mutex_unlock(&m->philos[i].meal_lock);
	}
	return (0);
}

static int	is_dead(t_monitor *m)
{
	int	i;

	i = -1;
	while (++i < m->philo_amt)
	{
		pthread_mutex_lock(&m->philos[i].meal_lock);
		if (m->philos[i].eaten_enough == 1)
		{
			pthread_mutex_unlock(&m->philos[i].meal_lock);
			pthread_mutex_lock(&m->death_lock);
			m->death = 1;
			pthread_mutex_unlock(&m->death_lock);
			write_state(&m->philos[i], "died");
			return (1);
		}
		pthread_mutex_unlock(&m->philos[i].meal_lock);
	}
	return (0);
}

void	supervise(t_monitor *m)
{
	if (m->meals_to_eat == 0)
	{
		while (1)
		{
			if (is_dead(m))
				return ;
		}
	}
	while (1)
	{
		if (are_all_done_eating_or_dead(m))
			return ;
	}
}
