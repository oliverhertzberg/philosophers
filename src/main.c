/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:02:38 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/03/28 20:34:20 by ohertzbe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void    write_state(t_philos *p, char *state)
{
    size_t  timestamp;
    
    timestamp = get_time() - *p->start_time;
    pthread_mutex_lock(p->write_lock);
    printf("%zu %d %s\n", timestamp, p->philo_num, state);
    pthread_mutex_unlock(p->write_lock);
}

void    think(t_philos *p)
{
    write_state(p, "is thinking");
}

void    snooze(t_philos *p)
{
    write_state(p, "is sleeping");
    usleep(p->ms_to_sleep * 1000);
}

void    eat(t_philos *p)
{
    pthread_mutex_lock(p->r_fork);
    write_state(p, "has taken a fork");
    pthread_mutex_lock(p->l_fork);
    write_state(p, "has taken a fork");
    p->last_meal = get_time();
    p->meals_eaten++;
    write_state(p, "is eating");
    usleep(p->ms_to_eat * 1000);
    pthread_mutex_unlock(p->r_fork);
    pthread_mutex_unlock(p->l_fork);
}

void    *philosophize(void *philo)
{
    t_philos *p;

    p = (t_philos *)philo;
    if (p->philo_num % 2 == 0)
        usleep(1000);
    //while(!*p->death)
    //{
    eat(p);
    think(p);
    snooze(p);
    //}
    return (philo);
}

/*void    *supervise(void *monitor)
{
    t_monitor *m;

    m = (t_monitor *)monitor;
    while(1)
    {
        if (mortality_monitor() || satiety_sleuth())
            break ;
    }
    return (monitor);
}*/

size_t  get_time()
{
    struct timeval time;
    
    gettimeofday(&time, NULL);
    return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void    free_and_destroy(char *s, t_monitor *m, t_philos *p)
{
    int i;
    
    if (s)
        write(2, &s, ft_strlen(s));
    free(p);
    free(m->forks);
    i = -1;
    while (++i < p->philo_amt)
        pthread_mutex_destroy(&m->forks[i]);
    pthread_mutex_destroy(&m->write_lock);
}

int create_threads(t_philos *p, t_monitor *m)
{
    int i;
    
    //pthread_create(&m->monitor, NULL, &supervise, (void *)m);
    m->start_time = get_time();
    i = -1;
    while (++i < m->philo_amt)
        p[i].last_meal = m->start_time;
    i = -1;
    while (++i < m->philo_amt)
    {
        if (pthread_create(&p[i].philo_t, NULL, &philosophize, (void *)&p[i]) != 0)
        {
            while(--i >= 0)
                pthread_join(p[i].philo_t, NULL);
            return (free_and_destroy("failed to create thread\n", m, p), 1);
        }
    }
    i = -1;
    pthread_join(m->monitor, NULL);
    while (++i < m->philo_amt)
        pthread_join(p[i].philo_t, NULL);
    return (0);
}

int create_mutexes(t_monitor *m)
{
    int i;
    
    i = -1;
    while (++i < m->philo_amt)
    {
        if (pthread_mutex_init(&m->forks[i], NULL) != 0)
        {
            while (--i >= 0)
                pthread_mutex_destroy(&m->forks[i]);
            write(2, "failed to initialize mutex\n", 28);
            return (1);
        }
    }
    if (pthread_mutex_init(&m->write_lock, NULL) != 0)
    {
        i = -1;
        while (++i < m->philo_amt)
            pthread_mutex_destroy(&m->forks[i]);
        write(2, "failed to initialize mutex\n", 28);
        return (1);
    }
    return (0);
}

void get_arguments(t_monitor *m, char **argv, int argc)
{
    m->ms_to_die = ft_atoi(argv[2]);
    m->ms_to_eat = ft_atoi(argv[3]);
    m->ms_to_sleep = ft_atoi(argv[4]);
    if (argc == 6)
        m->meals_to_eat = ft_atoi(argv[5]); 
    else
        m->meals_to_eat = -1;
    m->death = 0;
    m->start_time = 0;
}

void    init_philosophers(t_philos *p, t_monitor *m, char **argv, int argc)
{
    int i;

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
        p[i].write_lock = &m->write_lock;
        if (i == 0)
            p[i].r_fork = &m->forks[m->philo_amt - 1];
        else
            p[i].r_fork = &m->forks[i - 1];
        p[i].l_fork = &m->forks[i];
        p[i].death = &m->death;
        p[i].start_time = &m->start_time;
    }
}

int is_numeric(char *s)
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

int check_arguments(char **argv, int argc)
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

int main(int argc, char **argv)
{
    t_monitor  monitor;
    t_philos *philos;
    
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
    if (create_mutexes(&monitor))
        return (free(philos), free(monitor.forks), 1);
    init_philosophers(philos, &monitor, argv, argc);
    if (create_threads(philos, &monitor))
        return (1);
    free_and_destroy(NULL, &monitor, philos);
    return (0);
}