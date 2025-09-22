/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:14:30 by achowdhu          #+#    #+#             */
/*   Updated: 2025/09/22 18:31:31 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philo_death(t_data *data, int i)
{
	long long	time;
	long long	last_meal;
	int			died;

	died = 0;
	time = get_time();
	pthread_mutex_lock(&data->philos[i].meal_mutex);
	last_meal = data->philos[i].last_meal;
	pthread_mutex_unlock(&data->philos[i].meal_mutex);
	if ((time - last_meal) > data->t_die)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (!data->someone_died)
		{
			data->someone_died = 1;
			pthread_mutex_lock(&data->print_mutex);
			printf("%lld %d died\n",
				time - data->start_time, data->philos[i].id);
			pthread_mutex_unlock(&data->print_mutex);
			died = 1;
		}
		pthread_mutex_unlock(&data->death_mutex);
	}
	return (died);
}

int	check_all_ate(t_data *data)
{
	int	i;
	int	all_ate;

	if (data->n_meals == -1)
		return (0);
	all_ate = 1;
	i = 0;
	while (i < data->n_philos)
	{
		pthread_mutex_lock(&data->meal_check_mutex);
		if (data->philos[i].meals_eaten < data->n_meals)
			all_ate = 0;
		pthread_mutex_unlock(&data->meal_check_mutex);
		i++;
	}
	return (all_ate);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	int		all_ate;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->n_philos)
		{
			if (check_philo_death(data, i))
				return (NULL);
			i++;
		}
		all_ate = check_all_ate(data);
		if (all_ate)
		{
			pthread_mutex_lock(&data->death_mutex);
			data->someone_died = 1;
			pthread_mutex_unlock(&data->death_mutex);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	if (ph->id % 2 == 0)
		usleep(1500);
	while (!should_stop(ph))
	{
		philo_eat(ph);
		if (should_stop(ph))
			break ;
		philo_rest(ph, "is sleeping", ph->data->t_sleep);
		if (should_stop(ph))
			break ;
		philo_rest(ph, "is thinking", 0);
	}
	return (NULL);
}
