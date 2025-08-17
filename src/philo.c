/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:07:16 by achowdhu          #+#    #+#             */
/*   Updated: 2025/08/17 18:16:13 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void print_action(t_philo *philo, char *action)
{
    pthread_mutex_lock(&philo->data->print_mutex);
    if (!philo->data->someone_died)
        printf("%lld %d %s\n", current_timestamp(), philo->id, action);
    pthread_mutex_unlock(&philo->data->print_mutex);
}

static void take_forks(t_philo *philo)
{
    pthread_mutex_lock(philo->left_fork);
    print_action(philo, "has taken a fork");
    pthread_mutex_lock(philo->right_fork);
    print_action(philo, "has taken a fork");
}

static void eat_sleep_think(t_philo *philo)
{
    philo->last_meal_time = current_timestamp();
    print_action(philo, "is eating");
    ft_usleep(philo->data->time_to_eat);
    philo->meals_eaten++;
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
    print_action(philo, "is sleeping");
    ft_usleep(philo->data->time_to_sleep);
    print_action(philo, "is thinking");
}

void *philosopher_routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    while (!philo->data->someone_died)
    {
        take_forks(philo);
        eat_sleep_think(philo);
    }
    return (NULL);
}
