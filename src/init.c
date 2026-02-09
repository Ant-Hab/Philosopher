/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:30:50 by achowdhu          #+#    #+#             */
/*   Updated: 2026/02/09 19:31:11 by achowdhu         ###   ########.fr       */
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
	if (data->n_philos <= 0 || data->t_die <= 0 || data->t_eat <= 0
		|| data->t_sleep <= 0 || (ac == 6 && data->n_meals <= 0))
		return (1);
	data->someone_died = 0;
	data->start_time = 0;
	return (0);
}

static int	cleanup_mutexes(t_data *data, int i)
{
	while (--i >= 0)
		pthread_mutex_destroy(&data->forks[i]);
	free(data->forks);
	data->forks = NULL;
	return (1);
}

int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->n_philos);
	if (!data->forks)
		return (1);
	i = -1;
	while (++i < data->n_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (cleanup_mutexes(data, i));
	}
	if (pthread_mutex_init(&data->print_mutex, NULL))
		return (cleanup_mutexes(data, data->n_philos));
	if (pthread_mutex_init(&data->death_mutex, NULL))
	{
		pthread_mutex_destroy(&data->print_mutex);
		return (cleanup_mutexes(data, data->n_philos));
	}
	return (0);
}

int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->n_philos);
	if (!data->philos)
		return (1);
	i = -1;
	while (++i < data->n_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal = get_time();
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->n_philos];
		data->philos[i].data = data;
		data->philos[i].thread = 0;
		if (pthread_mutex_init(&data->philos[i].meal_mutex, NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->philos[i].meal_mutex);
			return (free(data->philos), data->philos = NULL, 1);
		}
	}
	return (0);
}

int	init_data(t_data *data, int ac, char **av)
{
	data->forks = NULL;
	data->philos = NULL;
	if (parse_args(data, ac, av))
		return (1);
	if (init_mutexes(data))
		return (1);
	return (0);
}
