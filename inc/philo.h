/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:30:14 by achowdhu          #+#    #+#             */
/*   Updated: 2026/02/05 17:43:27 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_mutex;
	t_data			*data;
	pthread_t		thread;
}	t_philo;

struct s_data
{
	int				n_philos;
	long long		t_die;
	long long		t_eat;
	long long		t_sleep;
	int				n_meals;
	int				someone_died;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	t_philo			*philos;
	pthread_t		monitor;
};

/* Init & Parse */
int			init_data(t_data *data, int ac, char **av);
int			init_philos(t_data *data);

/* Utils */
int			ft_atoi(const char *str);
long long	get_time(void);
void		smart_sleep(long long ms, t_data *data);
void		free_data(t_data *data);

/* Actions */
void		print_action(t_philo *ph, char *msg);
int			philo_eat(t_philo *ph);
void		philo_think(t_philo *ph);

/* Routines */
void		*monitor_routine(void *arg);
void		*routine(void *arg);

#endif