/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 01:07:24 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/06/13 22:09:43 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	continue_sim(t_philos *p, pthread_mutex_t *m1, pthread_mutex_t *m2)
{
	pthread_mutex_lock(p->death_lock);
	if (*p->death == 1)
	{
		pthread_mutex_unlock(p->death_lock);
		if (m1)
			pthread_mutex_unlock(m1);
		if (m2)
			pthread_mutex_unlock(m2);
		return (0);
	}
	pthread_mutex_unlock(p->death_lock);
	return (1);
}

static int	think(t_philos *p)
{
	if (continue_sim(p, NULL, NULL) == 0)
		return (0);
	write_state(p, "is thinking");
	return (1);
}

static int	snooze(t_philos *p)
{
	if (continue_sim(p, NULL, NULL) == 0)
		return (0);
	write_state(p, "is sleeping");
	ft_usleep(p->ms_to_sleep);
	return (1);
}

static int	eat(t_philos *p)
{
	if (continue_sim(p, NULL, NULL) == 0)
		return (0);
	pthread_mutex_lock(p->r_fork);
	if (continue_sim(p, p->r_fork, NULL) == 0)
		return (0);
	write_state(p, "has taken a fork");
	pthread_mutex_lock(p->l_fork);
	if (continue_sim(p, p->r_fork, p->l_fork) == 0)
		return (0);
	write_state(p, "has taken a fork");
	pthread_mutex_lock(&p->meal_lock);
	p->last_meal = get_time();
	pthread_mutex_unlock(&p->meal_lock);
	if (continue_sim(p, p->r_fork, p->l_fork) == 0)
		return (0);
	write_state(p, "is eating");
	pthread_mutex_lock(&p->meal_lock);
	p->meals_eaten++;
	pthread_mutex_unlock(&p->meal_lock);
	ft_usleep(p->ms_to_eat);
	pthread_mutex_unlock(p->r_fork);
	pthread_mutex_unlock(p->l_fork);
	return (1);
}

void	*philosophize(void *philo)
{
	t_philos	*p;

	p = (t_philos *)philo;
	if (p->philo_amt == 1)
	{
		while (!*p->death)
			ft_usleep(1);
		return (philo);
	}
	pthread_mutex_lock(&p->meal_lock);
	p->last_meal = get_time();
	pthread_mutex_unlock(&p->meal_lock);
	if (p->philo_num % 2 == 0)
		ft_usleep(1);
	pthread_mutex_lock(p->death_lock);
	while (!*p->death)
	{
		pthread_mutex_unlock(p->death_lock);
		if (!eat(p) || !think(p) || !snooze(p))
			return (philo);
		pthread_mutex_lock(p->death_lock);
	}
	pthread_mutex_unlock(p->death_lock);
	return (philo);
}
