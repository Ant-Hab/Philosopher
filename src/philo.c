/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:30:34 by achowdhu          #+#    #+#             */
/*   Updated: 2026/01/15 22:21:47 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	handle_one_philo(t_data *data)
{
	data->start_time = get_time();
	printf("0 1 has taken a fork\n");
	usleep(data->t_die * 1000);
	printf("%lld 1 died\n", get_time() - data->start_time);
}

static int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				routine, &data->philos[i]))
			return (1);
		i++;
	}
	data->start_time = get_time();
	i = 0;
	while (i < data->n_philos)
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		data->philos[i].last_meal = data->start_time;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		i++;
	}
	if (pthread_create(&data->monitor, NULL, monitor_routine, data))
		return (1);
	return (0);
}

static void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
		pthread_join(data->philos[i++].thread, NULL);
	pthread_join(data->monitor, NULL);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
		return (printf("Error: wrong arguments\n"), 1);
	if (init_data(&data, ac, av) || init_philos(&data))
		return (printf("Error: init failed\n"), 1);
	if (data.n_philos == 1)
		handle_one_philo(&data);
	else
	{
		if (create_threads(&data))
		{
			free_data(&data);
			return (printf("Error: thread creation failed\n"), 1);
		}
		join_threads(&data);
	}
	free_data(&data);
	return (0);
}
