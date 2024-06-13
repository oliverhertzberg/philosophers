/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 01:10:11 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/06/13 22:15:47 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	is_dead(t_monitor *m, t_philos *p)
{
	int	i;

	i = -1;
	while (++i < m->philo_amt)
	{
		pthread_mutex_lock(&p[i].meal_lock);
		if ((get_time() - p[i].last_meal) > m->ms_to_die)
		{
			pthread_mutex_unlock(&p[i].meal_lock);
			pthread_mutex_lock(p->death_lock);
			*p->death = 1;
			write_state(&p[i], "died");
			pthread_mutex_unlock(p->death_lock);
			return (1);
		}
		pthread_mutex_unlock(&p[i].meal_lock);
	}
	return (0);
}

static int	all_ate(t_philos *p, t_monitor *m)
{
	int	i;

	i = -1;
	while (++i < p->philo_amt)
	{
		pthread_mutex_lock(&p[i].meal_lock);
		if (p[i].meals_eaten != m->meals_to_eat)
		{
			pthread_mutex_unlock(&p[i].meal_lock);
			return (0);
		}
		pthread_mutex_unlock(&p[i].meal_lock);
	}
	pthread_mutex_lock(p->death_lock);
	*p->death = 1;
	pthread_mutex_unlock(p->death_lock);
	return (1);
}

void	supervise(t_monitor *m)
{
	if (m->philo_amt == 1)
	{
		ft_usleep(m->ms_to_die);
		pthread_mutex_lock(&m->death_lock);
		m->death = 1;
		pthread_mutex_unlock(&m->death_lock);
		write_state(&m->philos[0], "died");
		return ;
	}
	while (1)
	{
		if (is_dead(m, m->philos) == 1)
			return ;
		if (m->meals_to_eat && all_ate(m->philos, m))
			return ;
	}
}
