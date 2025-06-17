/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:15:30 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/17 16:23:18 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include "../libft/libft.h"
# include "../libft/printf/ft_printf.h"
# include <pthread.h>
# include <stdbool.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN    "\033[0;36m"
#define WHITE   "\033[0;37m"
#define RESET   "\033[0m"

typedef struct s_philosophers	t_philosopher;
typedef struct s_data			t_data;

struct							s_philosophers
{
	pthread_t					thread;
	int							id;
	bool						is_dead;
	int							time_to_die;
	int							time_to_eat;
	int							time_to_sleep;
	int							last_meal_time;
	long						last_meal;
	int							meals_eaten;
	int							forks1[2];
	int							forks2[2];
	pthread_mutex_t				*meal_lock;
	t_data						*data;
};
struct							s_data
{
	int							limit_meals;
	int							number_of_philosophers;
	int							time_to_die;
	int							time_to_eat;
	int							time_to_sleep;
	int							number_of_meals;
	int							simulation_has_stopped;
	long						start_t;
	pthread_mutex_t				*forks;
	pthread_mutex_t				*meal_lock;
	pthread_mutex_t				stop_lock;
	pthread_mutex_t				print_lock;
	t_philosopher				*philosophers;
};

int								check_args(int argc, char *argv[]);
int								set_data(t_data *data, int argc, char *argv[]);
void							*handle_error_philo(t_data *data, int code_err,
									int meg);
void							check_entered_input(char *arv[], int argc);
void							init_data(t_data *data);
#endif