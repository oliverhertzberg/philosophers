/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 01:14:39 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/06/13 22:14:28 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	init_monitor(t_monitor *m, int argc, char **argv)
{
	m->ms_to_die = ft_atoi(argv[2]);
	m->death = 0;
	m->philo_amt = ft_atoi(argv[1]);
	if (argc == 6)
		m->meals_to_eat = ft_atoi(argv[5]);
	else
		m->meals_to_eat = 0;
}

void	init_philosophers(t_philos *p, t_monitor *m, char **argv, int argc)
{
	int	i;

	init_monitor(m, argc, argv);
	i = -1;
	while (++i < m->philo_amt)
	{
		p[i].philo_num = i + 1;
		p[i].meals_eaten = 0;
		p[i].philo_amt = m->philo_amt;
		p[i].ms_to_eat = ft_atoi(argv[3]);
		p[i].ms_to_sleep = ft_atoi(argv[4]);
		p[i].last_meal = get_time();
		p[i].write_lock = &m->write_lock;
		p[i].death_lock = &m->death_lock;
		p[i].start_time = &m->start_time;
		if (i == 0)
			p[i].r_fork = &m->forks[m->philo_amt - 1];
		else
			p[i].r_fork = &m->forks[i - 1];
		p[i].l_fork = &m->forks[i];
		p[i].death = &m->death;
	}
}
