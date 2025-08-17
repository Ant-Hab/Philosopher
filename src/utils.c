/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:07:20 by achowdhu          #+#    #+#             */
/*   Updated: 2025/08/17 18:16:16 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long current_timestamp(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void ft_usleep(long long ms)
{
    long long start;

    start = current_timestamp();
    while ((current_timestamp() - start) < ms)
        usleep(100);
}

int ft_atoi(const char *str)
{
    int i;
    int res;

    i = 0;
    res = 0;
    while (str[i] >= '0' && str[i] <= '9')
    {
        res = res * 10 + (str[i] - '0');
        i++;
    }
    return (res);
}

void join_and_cleanup(t_philo *philos, t_data *data)
{
    int i;

    i = 0;
    while (i < data->n_philos)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }
    i = 0;
    while (i < data->n_philos)
    {
        pthread_mutex_destroy(&data->forks[i]);
        i++;
    }
    pthread_mutex_destroy(&data->print_mutex);
    free(data->forks);
    free(philos);
}
