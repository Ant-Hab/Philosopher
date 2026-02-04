/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:31:49 by achowdhu          #+#    #+#             */
/*   Updated: 2026/02/04 16:19:28 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	long	res;
	int		sign;

	res = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		str++;
	}
	return (res * sign);
}

long long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	smart_sleep(long long ms, t_data *data)
{
	long long	start;

	start = get_time();
	while (get_time() - start < ms)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (data->someone_died)
		{
			pthread_mutex_unlock(&data->death_mutex);
			return ;
		}
		pthread_mutex_unlock(&data->death_mutex);
		usleep(200);
	}
}

void	free_data(t_data *data)
{
	int	i;

	if (!data)
		return ;
	if (data->forks)
	{
		i = 0;
		while (i < data->n_philos)
			pthread_mutex_destroy(&data->forks[i++]);
		free(data->forks);
		data->forks = NULL;
	}
	if (data->philos)
	{
		i = 0;
		while (i < data->n_philos)
			pthread_mutex_destroy(&data->philos[i++].meal_mutex);
		free(data->philos);
		data->philos = NULL;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
}
