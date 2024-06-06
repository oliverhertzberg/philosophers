/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 01:07:24 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/06/06 18:08:28 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	think(t_philos *p)
{
	write_state(p, "is thinking");
}

void	snooze(t_philos *p)
{
	write_state(p, "is sleeping");
	usleep(p->ms_to_sleep * 1000);
}

int	eat(t_philos *p)
{
	pthread_mutex_lock(p->r_fork);
	write_state(p, "has taken a fork");
	pthread_mutex_lock(p->l_fork);
	write_state(p, "has taken a fork");
	pthread_mutex_lock(&p->meal_lock);
	p->last_meal = get_time() - p->end_meal;
	if (p->last_meal > p->ms_to_die)
	{
		pthread_mutex_unlock(p->r_fork);
		pthread_mutex_unlock(p->l_fork);
		p->eaten_enough = 1;
		return (pthread_mutex_unlock(&p->meal_lock), 0);
	}
	p->meals_eaten++;
	if (p->meals_to_eat && p->meals_eaten >= p->meals_to_eat)
		p->eaten_enough = 1;
	pthread_mutex_unlock(&p->meal_lock);
	write_state(p, "is eating");
	usleep(p->ms_to_eat * 1000);
	pthread_mutex_unlock(p->r_fork);
	pthread_mutex_unlock(p->l_fork);
	if (p->eaten_enough)
		return (0);
	p->end_meal = get_time();
	return (1);
}

void	*philosophize(void *philo)
{
	t_philos	*p;

	p = (t_philos *)philo;
	p->end_meal = *(p->start_time);
	if (p->philo_num % 2 == 0)
		usleep(1000);
	pthread_mutex_lock(p->death_lock);
	while (!*p->death)
	{
		pthread_mutex_unlock(p->death_lock);
		if (!(eat(p)))
			return (philo);
		think(p);
		snooze(p);
		pthread_mutex_lock(p->death_lock);
	}
	pthread_mutex_unlock(p->death_lock);
	return (philo);
}
