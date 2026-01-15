/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:30:50 by achowdhu          #+#    #+#             */
/*   Updated: 2026/01/15 22:21:27 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_args(t_data *data, int ac, char **av)
{
	data->n_philos = ft_atoi(av[1]);
	data->t_die = ft_atoi(av[2]);
	data->t_eat = ft_atoi(av[3]);
	data->t_sleep = ft_atoi(av[4]);
	data->n_meals = -1;
	if (ac == 6)
		data->n_meals = ft_atoi(av[5]);
	if (data->n_philos <= 0 || data->t_die <= 0
		|| data->t_eat <= 0 || data->t_sleep <= 0
		|| (ac == 6 && data->n_meals <= 0))
		return (1);
	data->someone_died = 0;
	data->start_time = 0;
	return (0);
}

static int	cleanup_mutex_fail(t_data *data, int i, int step)
{
	if (step >= 2)
		pthread_mutex_destroy(&data->print_mutex);
	if (step >= 1)
	{
		while (--i >= 0)
			pthread_mutex_destroy(&data->forks[i]);
	}
	free(data->forks);
	free(data->philos);
	return (1);
}

int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->n_philos);
	data->philos = malloc(sizeof(t_philo) * data->n_philos);
	if (!data->forks || !data->philos)
		return (1);
	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (cleanup_mutex_fail(data, i, 1));
		i++;
	}
	if (pthread_mutex_init(&data->print_mutex, NULL))
		return (cleanup_mutex_fail(data, i, 1));
	if (pthread_mutex_init(&data->death_mutex, NULL))
		return (cleanup_mutex_fail(data, i, 2));
	return (0);
}

int	init_data(t_data *data, int ac, char **av)
{
	if (parse_args(data, ac, av))
		return (1);
	if (init_mutexes(data))
		return (1);
	return (0);
}

int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal = get_time();
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->n_philos];
		data->philos[i].data = data;
		if (pthread_mutex_init(&data->philos[i].meal_mutex, NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->philos[i].meal_mutex);
			free_data(data);
			return (1);
		}
		i++;
	}
	return (0);
}
