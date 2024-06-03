/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 01:09:41 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/06/04 01:09:53 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void    write_state(t_philos *p, char *state)
{
    long long   timestamp;
    
    timestamp = get_time() - *p->start_time;
    pthread_mutex_lock(p->write_lock);
    printf("%lld %d %s\n", timestamp, p->philo_num, state);
    pthread_mutex_unlock(p->write_lock);
}
