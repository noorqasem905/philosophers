/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:15:30 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/29 00:22:38 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define RED     "\033[0;31m"
# define GREEN   "\033[0;32m"
# define YELLOW  "\033[0;33m"
# define BLUE    "\033[0;34m"
# define MAGENTA "\033[0;35m"
# define CYAN    "\033[0;36m"
# define WHITE   "\033[0;37m"
# define RESET   "\033[0m"

typedef struct s_philosophers	t_philosopher;
typedef struct s_data			t_data;

struct							s_philosophers
{
	pthread_t					thread;
	int							id;
	int							time_to_die;
	int							time_to_eat;
	int							time_to_sleep;
	int							last_meal_time;
	long						last_meal;
	int							meals_eaten;
	int							forks1[2];
	int							forks2[2];
	pthread_mutex_t				meal_lock;
	t_data						*data;
};
struct							s_data
{
	int									id;
	_Atomic int							limit_meals;
	int									number_of_philosophers;
	int									time_to_die;
	int									time_to_eat;
	int									time_to_sleep;
	int									number_of_meals;
	int									simulation_has_stopped;
	long								start_t;
	pthread_mutex_t						*forks;
	pthread_mutex_t						meal_lock;
	pthread_mutex_t						stop_lock;
	pthread_mutex_t						meal_limit;
	pthread_mutex_t						print_lock;
	t_philosopher						*philosophers;
};
long							get_time_in_milliseconds(void);
long							ft_atoi_long(const char *str);
int								set_data(t_data *data, int argc, char *argv[]);
int								simulation_has_stopped(t_philosopher *philo);
int								check_args(int argc, char *argv[]);
int								free_mutexes(int i, t_data *data);
int								is_died(t_data *data, int i);
int								eating(t_philosopher *philo);
int								ft_atoi(const char *str);
int								ft_isdigit(int c);
int								setup_mutex_creation(t_data *data);
void							*handle_error_philo(t_data *data, int code_err,
									int meg);
void							print_state(t_philosopher *philo,
									const char *state, int color);
void							usleep_custom(t_philosopher *philo,
									int time_to_sleep);
void							check_entered_input(char *arv[], int argc);
void							thinking(t_philosopher *philo);
void							take_fork(t_philosopher *philo);
void							sleeping(t_philosopher *philo);
void							init_data(t_data *data);
void							*routine(void *arg);
#endif