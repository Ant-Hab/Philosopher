/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:33:03 by achowdhu          #+#    #+#             */
/*   Updated: 2026/02/09 11:52:51 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_action(t_philo *ph, char *msg)
{
	pthread_mutex_lock(&ph->data->death_mutex);
	if (ph->data->someone_died)
	{
		pthread_mutex_unlock(&ph->data->death_mutex);
		return ;
	}
	pthread_mutex_lock(&ph->data->print_mutex);
	printf("%lld %d %s\n", get_time() - ph->data->start_time,
		ph->id, msg);
	pthread_mutex_unlock(&ph->data->print_mutex);
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
		smart_sleep(t_think * 0.5, ph->data);
	}
}

static void	eat_log_update(t_philo *ph)
{
	pthread_mutex_lock(&ph->meal_mutex);
	ph->last_meal = get_time();
	ph->meals_eaten++;
	pthread_mutex_unlock(&ph->meal_mutex);
}

static int	take_forks(t_philo *ph, pthread_mutex_t *f1, pthread_mutex_t *f2)
{
	pthread_mutex_lock(f1);
	print_action(ph, "has taken a fork");
	if (ph->data->n_philos == 1)
	{
		smart_sleep(ph->data->t_die, ph->data);
		pthread_mutex_unlock(f1);
		return (1);
	}
	pthread_mutex_lock(f2);
	print_action(ph, "has taken a fork");
	return (0);
}

int	philo_eat(t_philo *ph)
{
	pthread_mutex_t	*f1;
	pthread_mutex_t	*f2;

	f1 = ph->left_fork;
	f2 = ph->right_fork;
	if (f1 > f2)
	{
		f1 = ph->right_fork;
		f2 = ph->left_fork;
	}
	if (take_forks(ph, f1, f2))
		return (1);
	print_action(ph, "is eating");
	eat_log_update(ph);
	smart_sleep(ph->data->t_eat, ph->data);
	pthread_mutex_unlock(f2);
	pthread_mutex_unlock(f1);
	return (0);
}
