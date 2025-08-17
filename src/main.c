/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:07:13 by achowdhu          #+#    #+#             */
/*   Updated: 2025/08/17 18:16:11 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_philo *create_philos(t_data *data)
{
    t_philo *philos;

    philos = malloc(sizeof(t_philo) * data->n_philos);
    if (!philos)
        return (NULL);
    return (philos);
}

static int	init_data(t_data *data, int ac, char **av)
{
	data->n_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->n_meals = ft_atoi(av[5]);
	else
		data->n_meals = -1;
	data->someone_died = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->n_philos);
	if (!data->forks)
		return (1);
	return (0);
}

static void init_mutexes(t_data *data)
{
    int i;

    i = 0;
    while (i < data->n_philos)
    {
        pthread_mutex_init(&data->forks[i], NULL);
        i++;
    }
    pthread_mutex_init(&data->print_mutex, NULL);
}

int main(int ac, char **av)
{
    t_data  data;
    t_philo *philos;

    if (ac != 5 && ac != 6)
        return (printf("Invalid args\n"), 1);
    if (init_data(&data, ac, av))
        return (1);
    init_mutexes(&data);
    philos = create_philos(&data);
    if (!philos)
        return (free(data.forks), 1);
    init_philosophers(philos, &data);
    join_and_cleanup(philos, &data);
    return (0);
}
