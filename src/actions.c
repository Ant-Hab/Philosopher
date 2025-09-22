/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:11:47 by achowdhu          #+#    #+#             */
/*   Updated: 2025/09/22 18:31:33 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_action(t_philo *ph, char *msg)
{
	long long	time;

	pthread_mutex_lock(&ph->data->death_mutex);
	if (!ph->data->someone_died)
	{
		time = get_time() - ph->data->start_time;
		pthread_mutex_lock(&ph->data->print_mutex);
		printf("%lld %d %s\n", time, ph->id, msg);
		pthread_mutex_unlock(&ph->data->print_mutex);
	}
	pthread_mutex_unlock(&ph->data->death_mutex);
}

int	should_stop(t_philo *ph)
{
	int	stop;

	pthread_mutex_lock(&ph->data->death_mutex);
	stop = ph->data->someone_died;
	pthread_mutex_unlock(&ph->data->death_mutex);
	return (stop);
}

int	take_forks(t_philo *ph)
{
	if (should_stop(ph))
		return (0);
	pthread_mutex_lock(ph->left_fork);
	print_action(ph, "has taken a fork");
	if (should_stop(ph))
	{
		pthread_mutex_unlock(ph->left_fork);
		return (0);
	}
	pthread_mutex_lock(ph->right_fork);
	print_action(ph, "has taken a fork");
	if (should_stop(ph))
	{
		pthread_mutex_unlock(ph->right_fork);
		pthread_mutex_unlock(ph->left_fork);
		return (0);
	}
	return (1);
}

void	philo_eat(t_philo *ph)
{
	if (!take_forks(ph))
		return ;
	print_action(ph, "is eating");
	update_last_meal(ph);
	pthread_mutex_lock(&ph->data->meal_check_mutex);
	ph->meals_eaten++;
	pthread_mutex_unlock(&ph->data->meal_check_mutex);
	smart_sleep(ph->data->t_eat, ph->data);
	pthread_mutex_unlock(ph->right_fork);
	pthread_mutex_unlock(ph->left_fork);
}

void	philo_rest(t_philo *ph, char *action, long long duration)
{
	if (should_stop(ph))
		return ;
	print_action(ph, action);
	if (duration > 0)
		smart_sleep(duration, ph->data);
	else
		usleep(100);
}
