/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:02:14 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/21 20:14:17 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	handle_two_routine(t_philosopher *philo)
{
	pthread_mutex_unlock(&philo->data->meal_limit);
	pthread_mutex_lock(&philo->data->stop_lock);
	philo->data->simulation_has_stopped = 1;
	philo->data->id = philo->id;
	pthread_mutex_unlock(&philo->data->stop_lock);
}

int	handle_one_routine(t_philosopher *philo)
{
	if (philo->data->number_of_philosophers == 1)
	{
		pthread_mutex_lock(&philo->data->forks[philo->forks1[0]]);
		print_state(philo, "has taken a fork", 9);
		pthread_mutex_unlock(&philo->data->forks[philo->forks1[0]]);
		usleep_custom(philo, philo->time_to_die * 1000);
		return (1);
	}
	if (philo->id % 2 != 0)
		usleep(200);
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
		pthread_mutex_lock(&philo->data->meal_limit);
		if ((philo->data->limit_meals <= 0 && philo->data->number_of_meals
				!= -1) || philo->last_meal
			+ philo->time_to_die < get_time_in_milliseconds()
			- philo->data->start_t)
		{
			handle_two_routine(philo);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->data->meal_limit);
		if (!simulation_has_stopped(philo))
			sleeping(philo);
		if (!simulation_has_stopped(philo))
			thinging(philo);
	}
	return (NULL);
}
