/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   home_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:29:30 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/19 20:05:30 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	eating(t_philosopher *philo)
{
	if (simulation_has_stopped(philo))
		return (0);
	philo->meals_eaten++;
	pthread_mutex_lock(&philo->data->meal_lock);
	philo->last_meal = get_time_in_milliseconds() - philo->data->start_t;
	pthread_mutex_unlock(&philo->data->meal_lock);
	if (philo->data->number_of_meals != -1
		&& philo->meals_eaten >= philo->data->number_of_meals)
	{
		pthread_mutex_lock(&philo->data->meal_limit);
		philo->data->limit_meals--;
		pthread_mutex_unlock(&philo->data->meal_limit);
		// philo->meals_eaten = -100;
	}
	pthread_mutex_lock(&philo->data->print_lock);
	printf(GREEN "%ld Philosopher %d is eating\n" RESET, philo->last_meal,
		philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);
	usleep_custom(philo, philo->time_to_eat);
	return (0);
}

void	thinging(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->print_lock);
	printf(GREEN "%ld Philosopher %d is thinking\n" RESET,
		get_time_in_milliseconds() - philo->data->start_t, philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);
	usleep_custom(philo, 1);
}

void	sleeping(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->print_lock);
	printf(BLUE "%ld Philosopher %d is sleeping\n" RESET,
		get_time_in_milliseconds() - philo->data->start_t, philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);
	usleep_custom(philo, philo->time_to_sleep);
}

void	take_fork(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->data->forks[philo->forks1[0]]);
		print_state(philo, "has taken a fork", 9);
		pthread_mutex_lock(&philo->data->forks[philo->forks2[0]]);
		print_state(philo, "has taken a fork", 9);
	}
	else
	{
		pthread_mutex_lock(&philo->data->forks[philo->forks2[0]]);
		print_state(philo, "has taken a fork", 9);
		pthread_mutex_lock(&philo->data->forks[philo->forks1[0]]);
		print_state(philo, "has taken a fork", 9);
	}
	eating(philo);
	pthread_mutex_unlock(&philo->data->forks[philo->forks1[0]]);
	pthread_mutex_unlock(&philo->data->forks[philo->forks2[0]]);
}

void	*routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (philo->data->number_of_philosophers == 1)
	{
		pthread_mutex_lock(&philo->data->forks[philo->forks1[0]]);
		print_state(philo, "has taken a fork", 9);
		pthread_mutex_unlock(&philo->data->forks[philo->forks1[0]]);
		sleeping(philo);
		return (NULL);
	}
	if (philo->id % 2 != 0)
		usleep(200);
	while (!simulation_has_stopped(philo))
	{
		take_fork(philo);
		if ((philo->data->limit_meals <= 0 && philo->data->number_of_meals
				!= -1) || philo->last_meal
			+ philo->time_to_die < get_time_in_milliseconds()
			- philo->data->start_t)
		{
			pthread_mutex_lock(&philo->data->stop_lock);
			philo->data->simulation_has_stopped = 1;
			philo->data->id = philo->id;
			pthread_mutex_unlock(&philo->data->stop_lock);
			return (NULL);
		}
		if (!simulation_has_stopped(philo))
			sleeping(philo);
		if (!simulation_has_stopped(philo))
			thinging(philo);
	}
	return (NULL);
}
