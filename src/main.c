/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:02:38 by ohertzbe          #+#    #+#             */
/*   Updated: 2024/06/04 00:54:29 by ohertzbe         ###   ########.fr       */
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

void    think(t_philos *p)
{
    write_state(p, "is thinking");
}

void    snooze(t_philos *p)
{
    write_state(p, "is sleeping");
    usleep(p->ms_to_sleep * 1000);
}

int eat(t_philos *p)    
{
    pthread_mutex_lock(p->r_fork);
    write_state(p, "has taken a fork");
    pthread_mutex_lock(p->l_fork);
    write_state(p, "has taken a fork");
    pthread_mutex_lock(&p->meal_lock);
    p->last_meal = get_time() - p->end_meal;
    if (p->last_meal > p->ms_to_die)
    {
        pthread_mutex_unlock(&p->meal_lock);
        pthread_mutex_unlock(p->r_fork);
        pthread_mutex_unlock(p->l_fork);
        return (0);
    }
    p->meals_eaten++;
    pthread_mutex_unlock(&p->meal_lock);
    write_state(p, "is eating");
    usleep(p->ms_to_eat * 1000);
    pthread_mutex_unlock(p->r_fork);
    pthread_mutex_unlock(p->l_fork);
    p->end_meal = get_time();
    return (1);
}

void    *philosophize(void *philo)
{
    t_philos *p;

    p = (t_philos *)philo;
    p->end_meal = *(p->start_time);
    if (p->philo_num % 2 == 0)
        usleep(1000);
    pthread_mutex_lock(p->death_lock);
    while(!*p->death)
    {
        pthread_mutex_unlock(p->death_lock);
        if (!(eat(p)))
        {
            pthread_mutex_lock(p->death_lock);
            break ;
        }
        think(p);
        snooze(p);
        pthread_mutex_lock(p->death_lock);
    }
    pthread_mutex_unlock(p->death_lock);
    return (philo);
}

int is_dead_or_full(t_monitor *m)
{
    int i;

    i = -1;
    while (++i < m->philo_amt)
    {
        pthread_mutex_lock(&m->philos[i].meal_lock);
        if (m->philos[i].last_meal > m->ms_to_die)
        {
            pthread_mutex_unlock(&m->philos[i].meal_lock);
            pthread_mutex_lock(&m->death_lock);
            m->death = 1;
            pthread_mutex_unlock(&m->death_lock);
            return (write_state(&m->philos[i], "died"), 1);
        }
        else if (m->meals_to_eat && m->philos[i].meals_eaten >= m->meals_to_eat)
        {
            pthread_mutex_unlock(&m->philos[i].meal_lock);
            pthread_mutex_lock(&m->death_lock);
            m->death = 1;
            pthread_mutex_unlock(&m->death_lock);
            return (write_state(&m->philos[i], "ate too much"), 1);
        }
        pthread_mutex_unlock(&m->philos[i].meal_lock);
    }
    return (0);
}

void    supervise(t_monitor *m)
{   
    while(1)
    {
        if (is_dead_or_full(m))
            break ;
    }
}

long long  get_time()
{
    struct timeval tv;
    long long   timestamp;
    
    gettimeofday(&tv, NULL);
    timestamp = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
    return (timestamp);
}

void    destroy_meal_mutex(t_monitor *m)
{
    int i;

    i = -1;
    while (++i < m->philo_amt)
        pthread_mutex_destroy(&(m->philos[i].meal_lock));
}

void    destroy_fork_mutex(t_monitor *m)
{
    int i;

    i = -1;
    while (++i < m->philo_amt)
        pthread_mutex_destroy(&m->forks[i]);
}

void    free_and_destroy(char *s, t_monitor *m, t_philos *p)
{   
    if (s)
        write(2, &s, ft_strlen(s));
    destroy_fork_mutex(m);
    destroy_meal_mutex(m);
    pthread_mutex_destroy(&m->death_lock);
    pthread_mutex_destroy(&m->write_lock);
    free(p);
    free(m->forks);
}

int create_threads(t_philos *p, t_monitor *m)
{
    int i;
    
    m->start_time = get_time();
    i = -1;
    while (++i < m->philo_amt)
    {
        if (pthread_create(&p[i].philo_t, NULL, &philosophize, (void *)&p[i]) != 0)
        {
            m->death = 1;
            while(--i >= 0)
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

int init_meal_mutex(t_monitor *m)
{
    int i;

    i = -1;
    while (i++ < m->philo_amt)
    {
        if (pthread_mutex_init(&(m->philos[i].meal_lock), NULL) != 0)
        {
            while (--i >= 0)
                pthread_mutex_destroy(&(m->philos[i].meal_lock));
            write(2, "failed to initialize mutex\n", 28);
            return (0);
        }
    }
    return (1);
}

int init_fork_mutex(t_monitor *m)
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
            return (0);
        }
    }
    return (1);
}

int create_mutexes(t_monitor *m)
{
    if (!init_meal_mutex(m))
        return (0);
    if (!init_fork_mutex(m))
    {
        destroy_meal_mutex(m);
        return (0);
    }
    if (pthread_mutex_init(&m->write_lock, NULL) != 0)
    {
        destroy_meal_mutex(m);
        destroy_fork_mutex(m);
        write(2, "failed to initialize mutex\n", 28);
        return (0);
    }
    if (pthread_mutex_init(&m->death_lock, NULL) != 0)
    {
        destroy_meal_mutex(m);
        destroy_fork_mutex(m);
        pthread_mutex_destroy(&m->write_lock);
        write(2, "failed to initialize mutex\n", 28);
        return (0);
    }
    return (1);
}

void get_arguments(t_monitor *m, char **argv, int argc)
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
        p[i].last_meal = 0;
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
    if (!create_mutexes(&monitor))
        return (free(philos), free(monitor.forks), 1);
    init_philosophers(philos, &monitor, argv, argc);
    if (!create_threads(philos, &monitor))
        return (1);
    free_and_destroy(NULL, &monitor, philos);
    return (0);
}
