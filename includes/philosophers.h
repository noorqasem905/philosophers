/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:15:30 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/11 16:42:46 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include "../libft/libft.h"
# include "../libft/printf/ft_printf.h"
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philosophers	t_philosopher;
typedef struct s_data			t_data;

struct							s_philosophers
{
	pthread_t					thread;
	int							id;
	int							time_to_die;
	int							time_to_eat;
	int							time_to_sleep;
	int							number_of_meals;
	int							forks1[2];
	int							forks2[2];
	pthread_mutex_t				*print_lock;
};
struct							s_data
{
	int							iteration;
	int							number_of_philosophers;
	int							time_to_die;
	int							time_to_eat;
	int							time_to_sleep;
	int							number_of_meals;
	pthread_mutex_t				*forks;
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