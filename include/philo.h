/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:02:36 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/03/28 15:50:27 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

typedef struct s_philos
{
    pthread_t philo_t;
    pthread_mutex_t *r_fork;
    pthread_mutex_t *l_fork;
    pthread_mutex_t *write_lock;
    unsigned int    philo_num;
    size_t last_meal;
    int meals_to_eat;
    int meals_eaten;
    long philo_amt;
    long ms_to_die;
    long ms_to_eat;
    long ms_to_sleep;
    int *death;
    size_t *start_time;
}   t_philos;

typedef struct s_monitor
{
    pthread_t monitor;
    pthread_mutex_t *forks;
    pthread_mutex_t write_lock;
    t_philos    *philos;
    long philo_amt;
    long ms_to_die;
    long ms_to_eat;
    long ms_to_sleep;
    long meals_to_eat;
    size_t start_time;
    int death;
}   t_monitor;

// utils
long            ft_atoi(const char *str);
size_t  ft_strlen(char *s);
size_t  get_time();


#endif