/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:07:10 by achowdhu          #+#    #+#             */
/*   Updated: 2025/09/22 13:59:42 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

/* ---------------- Structures ---------------- */

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
	int				all_ate;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	meal_check_mutex;
	t_philo			*philos;
	pthread_t		monitor;
};

/* ---------------- Init Functions ---------------- */

int		parse_args(t_data *data, int ac, char **av);
int		init_mutexes(t_data *data);
int		init_data(t_data *data, int ac, char **av);
int		init_philos(t_data *data);

/* ---------------- Utils ---------------- */

int			ft_atoi(const char *str);
long long	get_time(void);
void		smart_sleep(long long ms, t_data *data);
void		update_last_meal(t_philo *ph);
void		free_data(t_data *data);

/* ---------------- Actions ---------------- */

void	print_action(t_philo *ph, char *msg);
int		should_stop(t_philo *ph);
int		take_forks(t_philo *ph);
void	philo_eat(t_philo *ph);
void	philo_rest(t_philo *ph, char *action, long long duration);

/* ---------------- Routine ---------------- */

int		check_philo_death(t_data *data, int i);
int		check_all_ate(t_data *data);
void	*monitor_routine(void *arg);
void	*routine(void *arg);

#endif
