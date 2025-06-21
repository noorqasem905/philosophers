/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:34:35 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/21 19:31:38 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long	get_time_in_milliseconds(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

int	setup_mutex2_creation(t_data *data)
{
	if (pthread_mutex_init(&data->stop_lock, NULL) != 0)
	{
		perror("Failed to initialize stop mutex");
		pthread_mutex_destroy(&data->print_lock);
		free(data->forks);
		free(data);
		return (-1);
	}
	if (pthread_mutex_init(&data->meal_lock, NULL) != 0)
	{
		perror("Failed to initialize stop mutex");
		pthread_mutex_destroy(&data->print_lock);
		free(data->forks);
		free(data);
		return (-1);
	}
	if (pthread_mutex_init(&data->meal_limit, NULL) != 0)
	{
		perror("Failed to initialize stop mutex");
		pthread_mutex_destroy(&data->meal_limit);
		free(data->forks);
		free(data);
		return (-1);
	}
	return (0);
}

int	setup_mutex_creation(t_data *data)
{
	int	i;

	i = -1;
	data->forks = malloc(sizeof(t_philosopher) * data->number_of_philosophers);
	if (!data->forks)
		return (-1);
	while (++i < (data)->number_of_philosophers)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			perror("Failed to initialize mutex for forks");
			free(data->forks);
			free(data);
			return (-1);
		}
	}
	if (free_mutexes(i, data) < 0)
		return (-1);
	if (setup_mutex2_creation(data) < 0)
		return (-1);
	return (0);
}

int	simulation_has_stopped(t_philosopher *philo)
{
	int	stopped;

	pthread_mutex_lock(&philo->data->stop_lock);
	stopped = philo->data->simulation_has_stopped;
	pthread_mutex_unlock(&philo->data->stop_lock);
	return (stopped);
}

void	usleep_custom(t_philosopher *philo, int time_to_sleep)
{
	long	start;

	start = get_time_in_milliseconds();
	while (!simulation_has_stopped(philo) && get_time_in_milliseconds()
		- start < time_to_sleep)
	{
		if ((get_time_in_milliseconds() - philo->data->start_t
				- philo->last_meal) >= philo->time_to_die)
		{
			pthread_mutex_lock(&philo->data->stop_lock);
			philo->data->simulation_has_stopped = 1;
			pthread_mutex_unlock(&philo->data->stop_lock);
			return ;
		}
		usleep(1000);
	}
}
