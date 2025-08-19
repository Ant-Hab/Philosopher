/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:07:13 by achowdhu          #+#    #+#             */
/*   Updated: 2025/08/19 16:31:09 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_data		data;
	t_philo		*philos;
	pthread_t	monitor;

	if (ac != 5 && ac != 6)
		return (printf("Invalid args\n"), 1);
	if (init_data(&data, ac, av))
		return (1);
	init_mutexes(&data);
	philos = create_philos(&data);
	if (!philos)
		return (free(data.forks), 1);
	init_philosophers(philos, &data);
	pthread_create(&monitor, NULL, monitor_routine, philos);
	pthread_join(monitor, NULL);
	join_and_cleanup(philos, &data);
	return (0);
}
