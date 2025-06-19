/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:17:56 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/19 20:59:28 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	set_philo_data(t_data *data)
{
	int	i;

	i = -1;
	data->start_t = get_time_in_milliseconds();
	data->limit_meals = data->number_of_meals;
	data->simulation_has_stopped = 0;
	while (++i < data->number_of_philosophers)
	{
		data->philosophers[i].last_meal = 0; 
		data->philosophers[i].id = i;
		data->philosophers[i].time_to_die = data->time_to_die;
		data->philosophers[i].time_to_eat = data->time_to_eat;
		data->philosophers[i].time_to_sleep = data->time_to_sleep;
		data->philosophers[i].meals_eaten = 0;
		data->philosophers[i].forks1[0] = i;
		data->philosophers[i].forks2[0] = (i + 1)
			% data->number_of_philosophers;
		data->philosophers[i].meal_lock = &data->print_lock;
		data->philosophers[i].data = data;
		if (pthread_create(&data->philosophers[i].thread, NULL, routine,
				&data->philosophers[i]) != 0)
		{
			printf("Failed to allocate memory for philosophers");
			free(data->philosophers);
			return (-1);
		}
	}
	return (0);
}

int	thread_creation(t_data *data)
{
	int	i;

	i = -1;
	data->philosophers = malloc(sizeof(t_philosopher)
			* data->number_of_philosophers);
	if (!data->philosophers)
	{
		printf("Failed to allocate memory for philosophers\n");
		pthread_mutex_destroy(&data->print_lock);
		while (++i < data->number_of_philosophers)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
		free(data);
		return (-1);
	}
	i = -1;
	if (set_philo_data(data) < 0)
	{
		printf("Failed to set philosopher data");
		free(data->philosophers);
		pthread_mutex_destroy(&data->print_lock);
		while (++i < data->number_of_philosophers)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
		free(data->philosophers);
		free(data);
		return (-1);
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_data	*data;
	int		i;

	check_entered_input(argv, argc);
	data = malloc(sizeof(t_data));
	if (!data)
		handle_error_philo(NULL, 1, 5);
	init_data(data);
	if (set_data(data, argc, argv))
		handle_error_philo(data, 1, 2);
	if (setup_mutex_creation(data) < 0)
		return (-1);
	if (thread_creation(data) < 0)
		return (-1);
	i = 0;
	while (i < data->number_of_philosophers)
	{
		if (pthread_join(data->philosophers[i].thread, NULL) != 0)
		{
			printf("Failed to join thread");
			free(data->philosophers);
			return (-1);
		}
		i++;
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->meal_limit);
	pthread_mutex_destroy(&data->meal_lock);
	pthread_mutex_destroy(&data->stop_lock);
	if (data->limit_meals > 0 || data->number_of_meals == -1)
		printf(RED "%ld Philosopher %d has died\n" RESET,
			get_time_in_milliseconds() - data->start_t, data->id);
	free(data->forks);
	free(data->philosophers);
	free(data);
	return (0);
}
