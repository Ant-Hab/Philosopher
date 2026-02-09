/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:33:03 by achowdhu          #+#    #+#             */
/*   Updated: 2026/02/09 19:38:59 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_action(t_philo *ph, char *msg)
{
	pthread_mutex_lock(&ph->data->death_mutex);
	if (!ph->data->someone_died)
	{
		pthread_mutex_lock(&ph->data->print_mutex);
		printf("%lld %d %s\n", get_time() - ph->data->start_time,
			ph->id, msg);
		pthread_mutex_unlock(&ph->data->print_mutex);
	}
	pthread_mutex_unlock(&ph->data->death_mutex);
}

void	philo_think(t_philo *ph)
{
	long long	t_think;

	print_action(ph, "is thinking");
	if (ph->data->n_philos % 2 != 0)
	{
		t_think = (ph->data->t_eat * 2) - ph->data->t_sleep;
		if (t_think < 0)
			t_think = 0;
		smart_sleep(t_think * 0.42, ph->data);
	}
}

static void	eat_log_update(t_philo *ph)
{
	pthread_mutex_lock(&ph->meal_mutex);
	ph->last_meal = get_time();
	ph->meals_eaten++;
	pthread_mutex_unlock(&ph->meal_mutex);
}

int	philo_eat(t_philo *ph)
{
	pthread_mutex_t	*f1;
	pthread_mutex_t	*f2;

	f1 = ph->left_fork;
	f2 = ph->right_fork;
	if (ph->id % 2 == 0)
	{
		f1 = ph->right_fork;
		f2 = ph->left_fork;
	}
	pthread_mutex_lock(f1);
	print_action(ph, "has taken a fork");
	if (ph->data->n_philos == 1)
		return (pthread_mutex_unlock(f1), 1);
	pthread_mutex_lock(f2);
	print_action(ph, "has taken a fork");
	print_action(ph, "is eating");
	eat_log_update(ph);
	smart_sleep(ph->data->t_eat, ph->data);
	pthread_mutex_unlock(f2);
	pthread_mutex_unlock(f1);
	return (0);
}
