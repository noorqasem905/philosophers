/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:02:14 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/29 00:21:16 by nqasem           ###   ########.fr       */
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

int	is_died(t_data *data, int i)
{
	long	current_time;

	current_time = get_time_in_milliseconds() - data->start_t;
	if (current_time - data->philosophers[i].last_meal
		> data->philosophers[i].time_to_die
		|| (data->limit_meals <= 0 && data->number_of_meals != -1))
	{
		data->simulation_has_stopped = 1;
		data->id = data->philosophers[i].id;
		pthread_mutex_unlock(&data->stop_lock);
		pthread_mutex_unlock(&data->philosophers[i].meal_lock);
		return (-1);
	}
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
		if (!simulation_has_stopped(philo))
			sleeping(philo);
		if (!simulation_has_stopped(philo))
			thinking(philo);
	}
	return (NULL);
}
