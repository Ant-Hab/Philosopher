/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:31:32 by achowdhu          #+#    #+#             */
/*   Updated: 2026/01/15 22:31:28 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philo_death(t_data *data, int i)
{
	long long	time;
	long long	last_meal;

	pthread_mutex_lock(&data->philos[i].meal_mutex);
	last_meal = data->philos[i].last_meal;
	pthread_mutex_unlock(&data->philos[i].meal_mutex);
	time = get_time();
	if ((time - last_meal) > data->t_die)
	{
		pthread_mutex_lock(&data->death_mutex);
		data->someone_died = 1;
		pthread_mutex_unlock(&data->death_mutex);
		pthread_mutex_lock(&data->print_mutex);
		printf("%lld %d died\n", time - data->start_time, data->philos[i].id);
		pthread_mutex_unlock(&data->print_mutex);
		return (1);
	}
	return (0);
}

int	check_all_ate(t_data *data)
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

static int	run_checks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		if (check_philo_death(data, i))
			return (1);
		i++;
	}
	if (check_all_ate(data))
		return (1);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!data->start_time)
		usleep(100);
	usleep(1000);
	while (1)
	{
		if (run_checks(data))
			return (NULL);
		pthread_mutex_lock(&data->death_mutex);
		if (data->someone_died)
		{
			pthread_mutex_unlock(&data->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->death_mutex);
		usleep(1000);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	while (!ph->data->start_time)
		usleep(100);
	if (ph->id % 2 == 0)
		usleep(1000);
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
		philo_rest(ph, "is sleeping", ph->data->t_sleep);
		philo_rest(ph, "is thinking", 0);
	}
	return (NULL);
}
