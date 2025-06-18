/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:34:35 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/18 16:34:51 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long	get_time_in_milliseconds(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000L) + (tv.tv_usec / 1000L);
}

int	setup_mutex_creation(t_data *data)
{
	int	i;

	i = -1;
	data->forks = malloc(sizeof(t_philosopher) * data->number_of_philosophers);
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
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
	{
		perror("Failed to initialize print mutex");
		free(data->forks);
		free(data);
		return (-1);
	}
	if (pthread_mutex_init(&data->stop_lock, NULL) != 0)
	{
		perror("Failed to initialize stop mutex");
		pthread_mutex_destroy(&data->print_lock);
		free(data->forks);
		free(data);
		return (-1);
	}
	return (0);
}

int simulation_has_stopped(t_philosopher *philo)
{
    int stopped;

    pthread_mutex_lock(&philo->data->stop_lock);
    stopped = philo->data->simulation_has_stopped;
    pthread_mutex_unlock(&philo->data->stop_lock);

    return stopped;
}

void	print_state(t_philosopher *philo, const char *state, int color)
{
	if (simulation_has_stopped(philo))
		return ;
	pthread_mutex_lock(&philo->data->print_lock);
	philo->last_meal = get_time_in_milliseconds() - philo->data->start_t;
	if (color == 1)
		printf(RED "%ld Philosopher %d %s\n" RESET, get_time_in_milliseconds() - philo->data->start_t, philo->id, state);
	else if (color == 2)
		printf(GREEN "%ld Philosopher %d %s\n" RESET, get_time_in_milliseconds() - philo->data->start_t, philo->id, state);
	else if (color == 3)
		printf(BLUE "%ld Philosopher %d %s\n" RESET, get_time_in_milliseconds() - philo->data->start_t, philo->id, state);
	else
		printf(WHITE "%ld Philosopher %d %s\n" RESET, get_time_in_milliseconds() - philo->data->start_t, philo->id, state);
	pthread_mutex_unlock(&philo->data->print_lock);
}

void	usleep_custom(t_philosopher *philo, int time_to_sleep)
{
	long	start;

	start = get_time_in_milliseconds();
	while (!simulation_has_stopped(philo) &&
	       get_time_in_milliseconds() - start < time_to_sleep)
	{
		if ((get_time_in_milliseconds() - philo->data->start_t - philo->last_meal) >= philo->time_to_die)
		{
			pthread_mutex_lock(&philo->data->stop_lock);
			philo->data->simulation_has_stopped = 1;
			pthread_mutex_unlock(&philo->data->stop_lock);
			return ;
		}
		usleep(100);
	}
}
