/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 01:12:36 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/06/16 13:31:44 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	destroy_meal_mutex(t_monitor *m)
{
	int	i;

	i = -1;
	while (++i < m->philo_amt)
		pthread_mutex_destroy(&(m->philos[i].meal_lock));
}

void	destroy_fork_mutex(t_monitor *m)
{
	int	i;

	i = -1;
	while (++i < m->philo_amt)
		pthread_mutex_destroy(&m->forks[i]);
}

void	free_and_destroy(char *s, t_monitor *m, t_philos *p)
{
	(void)m;
	(void)p;
	if (s)
		write(2, &s, ft_strlen(s));
	destroy_fork_mutex(m);
	destroy_meal_mutex(m);
	pthread_mutex_destroy(&m->death_lock);
	pthread_mutex_destroy(&m->write_lock);
	free(p);
	free(m->forks);
}
