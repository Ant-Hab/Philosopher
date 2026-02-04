/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:31:32 by achowdhu          #+#    #+#             */
/*   Updated: 2026/02/04 17:40:51 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	wait_for_start(t_data *data)
{
	long long	s_t;

	pthread_mutex_lock(&data->death_mutex);
	s_t = data->start_time;
	pthread_mutex_unlock(&data->death_mutex);
	while (s_t == 0)
	{
		usleep(100);
		pthread_mutex_lock(&data->death_mutex);
		s_t = data->start_time;
		pthread_mutex_unlock(&data->death_mutex);
	}
}

static int	check_philo_death(t_data *data, int i)
{
	long long	time;
	long long	last_meal;

	pthread_mutex_lock(&data->philos[i].meal_mutex);
	last_meal = data->philos[i].last_meal;
	pthread_mutex_unlock(&data->philos[i].meal_mutex);
	time = get_time();
	pthread_mutex_lock(&data->death_mutex);
	if ((time - last_meal) > data->t_die)
	{
		data->someone_died = 1;
		pthread_mutex_lock(&data->print_mutex);
		printf("%lld %d died\n", time - data->start_time, data->philos[i].id);
		pthread_mutex_unlock(&data->print_mutex);
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->death_mutex);
	return (0);
}

static int	check_all_ate(t_data *data)
{
	int	i;
	int	finished;

	if (data->n_meals == -1)
		return (0);
	i = 0;
	finished = 0;
	while (i < data->n_philos)
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		if (data->philos[i].meals_eaten >= data->n_meals)
			finished++;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		i++;
	}
	if (finished == data->n_philos)
	{
		pthread_mutex_lock(&data->death_mutex);
		data->someone_died = 1;
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	wait_for_start(data);
	while (1)
	{
		if (check_all_ate(data))
			break ;
		i = -1;
		while (++i < data->n_philos)
			if (check_philo_death(data, i))
				return (NULL);
		usleep(500);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	wait_for_start(ph->data);
	if (ph->id % 2 == 0)
		smart_sleep(ph->data->t_eat * 0.9, ph->data);
	while (1)
	{
		pthread_mutex_lock(&ph->data->death_mutex);
		if (ph->data->someone_died)
		{
			pthread_mutex_unlock(&ph->data->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&ph->data->death_mutex);
		if (philo_eat(ph))
			break ;
		print_action(ph, "is sleeping");
		smart_sleep(ph->data->t_sleep, ph->data);
		philo_think(ph);
	}
	return (NULL);
}
