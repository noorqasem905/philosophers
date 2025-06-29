/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_philo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:16:13 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/29 08:28:52 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	mutex_handler(t_data *data, int which)
{
	int	i;

	i = -1;
	if (which > 0)
		printf("Failed to initialize stop mutex\n");
	else
		printf("Failed to initialize stop forks\n");
	if (which > 0)
		while (++i < data->number_of_philosophers)
			pthread_mutex_destroy(&data->forks[i]);
	if (which > 1)
		pthread_mutex_destroy(&data->print_lock);
	if (which > 2)
		pthread_mutex_destroy(&data->stop_lock);
	if (which > 3)
		pthread_mutex_destroy(&data->meal_lock);
	free(data->forks);
	free(data);
}

int	free_mutexes(int i, t_data *data)
{
	if (i < data->number_of_philosophers)
	{
		mutex_handler(data, 0);
		return (-1);
	}
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
	{
		mutex_handler(data, 1);
		return (-1);
	}
	return (0);
}

void	print_state(t_philosopher *philo, const char *state, int color)
{
	if (simulation_has_stopped(philo))
		return ;
	pthread_mutex_lock(&philo->data->stop_lock);
	philo->last_meal = get_time_in_milliseconds() - philo->data->start_t;
	pthread_mutex_unlock(&philo->data->stop_lock);
	pthread_mutex_lock(&philo->data->print_lock);
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
