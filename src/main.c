/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:02:38 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/06/13 20:02:16 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	create_threads(t_philos *p, t_monitor *m)
{
	int	i;

	i = -1;
	while (++i < m->philo_amt)
	{
		if (pthread_create
			(&p[i].philo_t, NULL, &philosophize, (void *)&p[i]) != 0)
		{
			m->death = 1;
			while (--i >= 0)
				pthread_join(p[i].philo_t, NULL);
			free_and_destroy("failed to create thread\n", m, p);
			return (0);
		}
	}
	supervise(m);
	i = -1;
	while (++i < m->philo_amt)
		pthread_join(p[i].philo_t, NULL);
	free_and_destroy(NULL, m, p);
	return (1);
}

static int	is_numeric(char *s)
{
	if (!s)
		return (0);
	if (*s == '-' || *s == '+')
		s++;
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return (0);
		s++;
	}
	return (1);
}

static int	argument_error(char **argv, int argc)
{
	int	err;

	err = 0;
	if (argc < 5 || argc > 6)
	{
		write(2, "Error: Invalid number of arguments\n", 36);
		return (1);
	}
	if (!err && (!is_numeric(argv[1]) || ft_atoi(argv[1]) <= 0
			|| ft_atoi(argv[1]) > 200))
		err = 1;
	if (!err && (!is_numeric(argv[2]) || ft_atoi(argv[2]) <= 0))
		err = 1;
	if (!err && (!is_numeric(argv[3]) || ft_atoi(argv[3]) <= 0))
		err = 1;
	if (!err && (!is_numeric(argv[4]) || ft_atoi(argv[4]) <= 0))
		err = 1;
	if (!err && argc == 6 && ((!is_numeric(argv[5]) || ft_atoi(argv[5]) <= 0)))
		err = 1;
	if (!err)
		return (0);
	write (2, "Error: Invalid arguments\n", 26);
	return (1);
}

static void	free_mallocs(t_philos *philos, pthread_mutex_t *forks)
{
	if (philos)
		free(philos);
	if (forks)
		free(forks);
}

int	main(int argc, char **argv)
{
	t_monitor	monitor;
	t_philos	*philos;

	if (argument_error(argv, argc))
		return (1);
	monitor.philo_amt = ft_atoi(argv[1]);
	philos = malloc(sizeof(t_philos) * monitor.philo_amt);
	if (!philos)
		return (1);
	monitor.philos = philos;
	monitor.forks = malloc(sizeof(pthread_mutex_t) * monitor.philo_amt);
	if (!monitor.forks)
	{
		free_mallocs(philos, NULL);
		return (1);
	}
	if (!create_mutexes(&monitor))
	{
		free_mallocs(philos, monitor.forks);
		return (1);
	}
	init_philosophers(philos, &monitor, argv, argc);
	if (!create_threads(philos, &monitor))
		return (1);
	return (0);
}
