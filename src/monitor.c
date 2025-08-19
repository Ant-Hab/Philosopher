/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:14:30 by achowdhu          #+#    #+#             */
/*   Updated: 2025/08/19 17:16:52 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_philosopher_status(t_philo *philo, int *finished)
{
	long long	now;

	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		return (1);
	}
	now = current_timestamp();
	if ((now - philo->last_meal_time) >= philo->data->time_to_die)
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%lld %d died\n", now, philo->id);
		pthread_mutex_unlock(&philo->data->print_mutex);
		philo->data->someone_died = 1;
		pthread_mutex_unlock(&philo->data->death_mutex);
		return (1);
	}
	if (philo->data->n_meals > 0 && philo->meals_eaten >= philo->data->n_meals)
		(*finished)++;
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (0);
}

static int	check_all_philosophers(t_philo *philos, t_data *data)
{
	int	i;
	int	finished;
	int	death_detected;

	i = 0;
	finished = 0;
	death_detected = 0;
	while (i < data->n_philos)
	{
		if (check_philosopher_status(&philos[i], &finished))
		{
			death_detected = 1;
			break ;
		}
		i++;
	}
	if (death_detected)
		return (-1);
	return (finished);
}

static int	check_meals_completion(t_data *data, int finished)
{
	if (data->n_meals > 0 && finished == data->n_philos)
	{
		pthread_mutex_lock(&data->death_mutex);
		data->someone_died = 1;
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	return (0);
}

static int	check_simulation_end(t_data *data)
{
	int	ended;

	pthread_mutex_lock(&data->death_mutex);
	ended = data->someone_died;
	pthread_mutex_unlock(&data->death_mutex);
	return (ended);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philos;
	t_data	*data;
	int		finished;

	philos = (t_philo *)arg;
	data = philos[0].data;
	while (1)
	{
		finished = check_all_philosophers(philos, data);
		if (finished == -1)
			break ;
		if (check_meals_completion(data, finished))
			break ;
		usleep(500);
		if (check_simulation_end(data))
			break ;
	}
	return (NULL);
}
