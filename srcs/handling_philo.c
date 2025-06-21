/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_philo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:16:13 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/21 17:35:42 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	free_mutexes(int i, t_data *data)
{
	if (i < data->number_of_philosophers)
	{
		free(data->forks);
		free(data);
		return (-1);
	}
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
	{
		perror("Failed to initialize print mutex");
		free(data->forks);
		free(data);
		return (-1);
	}
	return (0);
}

void	print_state(t_philosopher *philo, const char *state, int color)
{
	if (simulation_has_stopped(philo))
		return ;
	pthread_mutex_lock(&philo->data->print_lock);
	philo->last_meal = get_time_in_milliseconds() - philo->data->start_t;
	if (color == 1)
		printf(RED "%ld Philosopher %d %s\n" RESET, get_time_in_milliseconds()
			- philo->data->start_t, philo->id, state);
	else if (color == 2)
		printf(GREEN "%ld Philosopher %d %s\n" RESET, get_time_in_milliseconds()
			- philo->data->start_t, philo->id, state);
	else if (color == 3)
		printf(BLUE "%ld Philosopher %d %s\n" RESET, get_time_in_milliseconds()
			- philo->data->start_t, philo->id, state);
	else
		printf(WHITE "%ld Philosopher %d %s\n" RESET, get_time_in_milliseconds()
			- philo->data->start_t, philo->id, state);
	pthread_mutex_unlock(&philo->data->print_lock);
}

void	err_mess(int msg)
{
	if (msg == 1)
		printf
		("Error: Invalid number of arguments.Expected 4 or 5 arguments.\n");
	else if (msg == 2)
		printf
		("Error: Invalid argument. All arguments must be valid integers.\n");
	else if (msg == 3)
		printf("Error: Number of philosophers must be between 1 and 200.\n");
	else if (msg == 4)
		printf("Error: Time values must be greater than or equal to 60.\n");
	else if (msg == 5)
		printf("Error: Memory allocation failed.\n");
}

void	*handle_error_philo(t_data *data, int code_err, int meg)
{
	err_mess(meg);
	if (data)
		free(data);
	exit(code_err * -1);
}
