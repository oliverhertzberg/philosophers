/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:02:36 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/03/22 16:58:51 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

typedef struct s_args
{
    pthread_t *philo_t;
    pthread_mutex_t *mutexes;
    long philo_amt;
    long ms_die;
    long ms_eat;
    long ms_sleep;
    long meal_amt;
    int i;
}   t_args;

long            ft_atoi(const char *str);
unsigned int    ft_strlen(char *s);


#endif