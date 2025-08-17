/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:07:10 by achowdhu          #+#    #+#             */
/*   Updated: 2025/08/17 18:16:15 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data
{
    int             n_philos;
    long long       time_to_die;
    long long       time_to_eat;
    long long       time_to_sleep;
    int             n_meals;
    int             someone_died;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
}   t_data;

typedef struct s_philo
{
    int         id;
    int         meals_eaten;
    long long   last_meal_time;
    pthread_t   thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_data      *data;
}   t_philo;

// main.c
int         ft_atoi(const char *str);
long long   current_timestamp(void);
void        init_philosophers(t_philo *philos, t_data *data);

// philo.c
void        *philosopher_routine(void *arg);
void        print_action(t_philo *philo, char *action);

// utils.c
void        ft_usleep(long long ms);
void        join_and_cleanup(t_philo *philos, t_data *data);

#endif
