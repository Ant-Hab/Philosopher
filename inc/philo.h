/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:07:10 by achowdhu          #+#    #+#             */
/*   Updated: 2025/08/19 14:32:43 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_data
{
	int				n_philos;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				n_meals;
	int				someone_died;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}	t_philo;

long long	current_timestamp(void);
long long	get_time(void);
void		ft_usleep(long time_in_ms);
int			ft_atoi(const char *str);
void		print_action(t_philo *philo, char *action);
void		join_and_cleanup(t_philo *philos, t_data *data);
t_philo	    *create_philos(t_data *data);
void		*philosopher_routine(void *arg);
void		*monitor_routine(void *arg);
int			init_data(t_data *data, int ac, char **av);
void		init_mutexes(t_data *data);
void		init_philosophers(t_philo *philos, t_data *data);

#endif
