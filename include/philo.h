/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:02:36 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/06/13 22:08:02 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <pthread.h>

typedef struct s_philos
{
	pthread_t		philo_t;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	*death_lock;
	unsigned int	philo_num;
	long long		last_meal;
	long long		*start_time;
	int				meals_eaten;
	long			philo_amt;
	long			ms_to_eat;
	long			ms_to_sleep;
	int				*death;
}	t_philos;

typedef struct s_monitor
{
	pthread_t		monitor;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	death_lock;
	t_philos		*philos;
	long long		start_time;
	long			philo_amt;
	long			ms_to_die;
	long			meals_to_eat;
	int				death;
}	t_monitor;

/* cleaning.c */
void		destroy_meal_mutex(t_monitor *m);
void		destroy_fork_mutex(t_monitor *m);
void		free_and_destroy(char *s, t_monitor *m, t_philos *p);
/* create_mutexes.c*/
int			init_meal_mutex(t_monitor *m);
int			init_fork_mutex(t_monitor *m);
int			create_mutexes(t_monitor *m);
/* get_time.c */
long long	get_time(void);
/* init_philos.c */
void		init_philosophers(t_philos *p, t_monitor *m, char **argv, int argc);
/* philo_functions.c */
void		*philosophize(void *philo);
/* supervisor.c */
void		supervise(t_monitor *m);
/* utils .c */
long		ft_atoi(const char *str);
size_t		ft_strlen(char *s);
int			ft_usleep(size_t milliseconds);
/* write_state.c */
void		write_state(t_philos *p, char *state);

#endif
