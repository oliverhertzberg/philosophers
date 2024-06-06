/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:02:38 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/06/06 18:06:04 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	create_threads(t_philos *p, t_monitor *m)
{
	int	i;

	m->start_time = get_time();
	i = -1;
	while (++i < m->philo_amt)
	{
		if (pthread_create
			(&p[i].philo_t, NULL, &philosophize, (void *)&p[i]) != 0)
		{
			m->death = 1;
			while (--i >= 0)
				pthread_join(p[i].philo_t, NULL);
			return (free_and_destroy("failed to create thread\n", m, p), 0);
		}
	}
	supervise(m);
	i = -1;
	while (++i < m->philo_amt)
		pthread_join(p[i].philo_t, NULL);
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

static int	check_arguments(char **argv, int argc)
{
	if (argc < 5 || argc > 6)
		return (write(2, "Error: Invalid number of arguments\n", 36), 1);
	if (!is_numeric(argv[1]) || ft_atoi(argv[1]) <= 0 || ft_atoi(argv[1]) > 200)
		return (write(2, "Error: Invalid number of philosophers\n", 39), 1);
	if (!is_numeric(argv[2]) || ft_atoi(argv[2]) <= 0)
		return (write(2, "Error: Invalid time to die\n", 28), 1);
	if (!is_numeric(argv[3]) || ft_atoi(argv[3]) <= 0)
		return (write(2, "Error: Invalid time to eat\n", 28), 1);
	if (!is_numeric(argv[4]) || ft_atoi(argv[4]) <= 0)
		return (write(2, "Error: Invalid time to sleep\n", 30), 1);
	if (argc == 6 && (!is_numeric(argv[5]) || ft_atoi(argv[5]) <= 0))
		return (write(2, "Error: Invalid argument for times to eat\n", 42), 1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_monitor	monitor;
	t_philos	*philos;

	if (check_arguments(argv, argc))
		return (1);
	monitor.philo_amt = ft_atoi(argv[1]);
	philos = malloc(sizeof(t_philos) * monitor.philo_amt);
	if (!philos)
		return (write(2, "malloc for philos failed\n", 26), 1);
	monitor.philos = philos;
	monitor.forks = malloc(sizeof(pthread_mutex_t) * monitor.philo_amt);
	if (!monitor.forks)
		return (free(philos), write(2, "malloc for forks failed\n", 25), 1);
	if (!create_mutexes(&monitor))
		return (free(philos), free(monitor.forks), 1);
	init_philosophers(philos, &monitor, argv, argc);
	if (!create_threads(philos, &monitor))
		return (1);
	free_and_destroy(NULL, &monitor, philos);
	return (0);
}
