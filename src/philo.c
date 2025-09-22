/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:07:16 by achowdhu          #+#    #+#             */
/*   Updated: 2025/09/22 18:32:04 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	cleanup_on_error(t_data *data, int created_threads)
{
	int	i;

	i = 0;
	while (i < data->n_philos && i < created_threads)
		pthread_detach(data->philos[i].thread);
	free_data(data);
}

static int	start_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, routine,
				&data->philos[i]) != 0)
		{
			cleanup_on_error(data, i);
			return (1);
		}
		i++;
	}
	if (pthread_create(&data->monitor, NULL, monitor_routine, data) != 0)
	{
		cleanup_on_error(data, i);
		return (1);
	}
	i = 0;
	while (i < data->n_philos)
		pthread_join(data->philos[i++].thread, NULL);
	pthread_join(data->monitor, NULL);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
		return (printf("Error: wrong arguments\n"), 1);
	if (init_data(&data, ac, av) || init_philos(&data))
		return (printf("Error: init failed\n"), 1);
	if (data.n_philos == 1)
	{
		printf("%lld 1 has taken a fork\n", get_time() - data.start_time);
		usleep(data.t_die * 1000);
		printf("%lld 1 died\n", get_time() - data.start_time);
		free_data(&data);
		return (0);
	}
	start_simulation(&data);
	free_data(&data);
	return (0);
}
