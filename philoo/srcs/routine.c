/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:02:14 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/28 18:14:19 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	handle_one_routine(t_philosopher *philo)
{
	if (philo->data->number_of_philosophers == 1)
	{
		pthread_mutex_lock(&philo->data->forks[philo->forks1[0]]);
		print_state(philo, "has taken a fork", 9);
		philo->data->id = philo->id;
		pthread_mutex_unlock(&philo->data->forks[philo->forks1[0]]);
		sleeping(philo);
		return (1);
	}
	if (philo->id % 2 != 0)
		usleep(200);
	return (0);
}

int	monitor_routine(t_philosopher *philo)
{
	int	i;

	while (!simulation_has_stopped(philo))
	{
		i = 0;
		while (i < philo->data->number_of_philosophers)
		{
			if (is_died(philo) < 0)
				return (-1);
			i++;
		}
		usleep(500);
	}
	return (0);
}

int	is_died(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->stop_lock);
	if ((philo->data->limit_meals <= 0 && philo->data->number_of_meals
			!= -1) || philo->last_meal
		+ philo->time_to_die < get_time_in_milliseconds()
		- philo->data->start_t)
	{
		philo->data->simulation_has_stopped = 1;
		philo->data->id = philo->id;
		pthread_mutex_unlock(&philo->data->stop_lock);
		return (-1);
	}
	pthread_mutex_unlock(&philo->data->stop_lock);
	return (0);
}

void	*routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (handle_one_routine(philo))
		return (NULL);
	while (!simulation_has_stopped(philo))
	{
		take_fork(philo);
		if (!monitor_routine(philo))
			return (NULL);
		if (!simulation_has_stopped(philo))
			sleeping(philo);
		if (!monitor_routine(philo))
			return (NULL);
		if (!simulation_has_stopped(philo))
			thinking(philo);
		if (!monitor_routine(philo))
			return (NULL);
	}
	return (NULL);
}
