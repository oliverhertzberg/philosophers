/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 17:38:19 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/03/27 16:20:38 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

size_t  ft_strlen(char *s)
{
    unsigned int    i;

    if (!s)
        return(0);
    i = 0;
    while (s[i])
        i++;
    return (i);
}
