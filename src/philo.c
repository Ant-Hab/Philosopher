/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:24:10 by achowdhu          #+#    #+#             */
/*   Updated: 2026/02/09 19:28:28 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		if (data->philos[i].thread != 0)
		{
			pthread_join(data->philos[i].thread, NULL);
			data->philos[i].thread = 0;
		}
		i++;
	}
	if (data->monitor != 0)
	{
		pthread_join(data->monitor, NULL);
		data->monitor = 0;
	}
}

static void	init_philo_start(t_data *data)
{
	int	i;

	i = -1;
	data->start_time = get_time();
	while (++i < data->n_philos)
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		data->philos[i].last_meal = data->start_time;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
	}
}

static int	create_threads(t_data *data)
{
	int	i;

	init_philo_start(data);
	i = -1;
	while (++i < data->n_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				routine, &data->philos[i]))
		{
			pthread_mutex_lock(&data->death_mutex);
			data->someone_died = 1;
			pthread_mutex_unlock(&data->death_mutex);
			return (join_threads(data), 1);
		}
	}
	if (pthread_create(&data->monitor, NULL, monitor_routine, data))
	{
		pthread_mutex_lock(&data->death_mutex);
		data->someone_died = 1;
		pthread_mutex_unlock(&data->death_mutex);
		return (join_threads(data), 1);
	}
	return (0);
}

static void	run_simulation(t_data *data)
{
	if (data->n_philos == 1)
	{
		data->start_time = get_time();
		printf("0 1 has taken a fork\n");
		smart_sleep(data->t_die, data);
		printf("%lld 1 died\n", (long long)data->t_die);
	}
	else
	{
		if (create_threads(data) == 0)
			join_threads(data);
	}
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
	{
		printf("Error: wrong arguments\n");
		return (1);
	}
	memset(&data, 0, sizeof(t_data));
	if (init_data(&data, ac, av) || init_philos(&data))
	{
		printf("Error: init failed\n");
		free_data(&data);
		return (1);
	}
	run_simulation(&data);
	free_data(&data);
	return (0);
}
