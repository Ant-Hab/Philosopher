/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:33:03 by achowdhu          #+#    #+#             */
/*   Updated: 2026/01/15 22:21:38 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_action(t_philo *ph, char *msg)
{
	long long	time;

	pthread_mutex_lock(&ph->data->print_mutex);
	pthread_mutex_lock(&ph->data->death_mutex);
	if (!ph->data->someone_died)
	{
		time = get_time() - ph->data->start_time;
		printf("%lld %d %s\n", time, ph->id, msg);
	}
	pthread_mutex_unlock(&ph->data->death_mutex);
	pthread_mutex_unlock(&ph->data->print_mutex);
}

void	philo_rest(t_philo *ph, char *action, long long duration)
{
	pthread_mutex_lock(&ph->data->death_mutex);
	if (ph->data->someone_died)
	{
		pthread_mutex_unlock(&ph->data->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&ph->data->death_mutex);
	print_action(ph, action);
	if (duration > 0)
		smart_sleep(duration, ph->data);
}

static int	check_death_forks(t_philo *ph, int forks_held)
{
	pthread_mutex_lock(&ph->data->death_mutex);
	if (ph->data->someone_died)
	{
		pthread_mutex_unlock(&ph->data->death_mutex);
		if (forks_held >= 1)
			pthread_mutex_unlock(ph->left_fork);
		if (forks_held == 2)
			pthread_mutex_unlock(ph->right_fork);
		return (1);
	}
	pthread_mutex_unlock(&ph->data->death_mutex);
	return (0);
}

int	philo_eat(t_philo *ph)
{
	pthread_mutex_lock(ph->left_fork);
	print_action(ph, "has taken a fork");
	if (check_death_forks(ph, 1))
		return (1);
	pthread_mutex_lock(ph->right_fork);
	print_action(ph, "has taken a fork");
	if (check_death_forks(ph, 2))
		return (1);
	pthread_mutex_lock(&ph->meal_mutex);
	ph->last_meal = get_time();
	ph->meals_eaten++;
	pthread_mutex_unlock(&ph->meal_mutex);
	print_action(ph, "is eating");
	smart_sleep(ph->data->t_eat, ph->data);
	pthread_mutex_unlock(ph->right_fork);
	pthread_mutex_unlock(ph->left_fork);
	return (0);
}
