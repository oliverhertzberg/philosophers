/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 01:14:39 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/06/06 17:58:07 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	get_arguments(t_monitor *m, char **argv, int argc)
{
	m->ms_to_die = ft_atoi(argv[2]);
	m->ms_to_eat = ft_atoi(argv[3]);
	m->ms_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		m->meals_to_eat = ft_atoi(argv[5]);
	else
		m->meals_to_eat = 0;
	m->death = 0;
	m->start_time = 0;
}

void	init_philosophers(t_philos *p, t_monitor *m, char **argv, int argc)
{
	int	i;

	get_arguments(m, argv, argc);
	i = -1;
	while (++i < m->philo_amt)
	{
		p[i].philo_num = i + 1;
		p[i].meals_eaten = 0;
		p[i].philo_amt = m->philo_amt;
		p[i].meals_to_eat = m->meals_to_eat;
		p[i].ms_to_die = m->ms_to_die;
		p[i].ms_to_eat = m->ms_to_eat;
		p[i].ms_to_sleep = m->ms_to_sleep;
		p[i].last_meal = 0;
		p[i].eaten_enough = 0;
		p[i].write_lock = &m->write_lock;
		p[i].death_lock = &m->death_lock;
		if (i == 0)
			p[i].r_fork = &m->forks[m->philo_amt - 1];
		else
			p[i].r_fork = &m->forks[i - 1];
		p[i].l_fork = &m->forks[i];
		p[i].death = &m->death;
		p[i].start_time = &m->start_time;
	}
}
