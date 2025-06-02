/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:17:56 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/02 16:55:00 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int main(int argc, char *argv[])
{
	t_data  *data;

	check_entered_input(argv, argc);
	data = malloc(sizeof(t_data));
	if (!data)
		handle_error_philo(NULL, 1, 5);
	init_data(data);
	if (set_data(data, argc, argv))
	{
		free(data);
		return (-1);
	}
	printf("NUMBER: %d\n", data->time_to_sleep);
	/* data->forks = malloc(sizeof(pthread_mutex_t) * data->number_of_philosophers);
	if (!data->forks)
	{
		perror("Failed to allocate memory for forks");
		free(data);
		return (1);
	}
	for (int i = 0; i < data->number_of_philosophers; i++)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			perror("Failed to initialize mutex for forks");
			free(data->forks);
			free(data);
			return (1);
		}
	}
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
	{
		perror("Failed to initialize print lock mutex");
		for (int i = 0; i < data->number_of_philosophers; i++)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
		free(data);
		return (1);
	}
	data->philosophers = malloc(sizeof(t_philosopher) * data->number_of_philosophers);
	if (!data->philosophers)
	{
		perror("Failed to allocate memory for philosophers");
		pthread_mutex_destroy(&data->print_lock);
		for (int i = 0; i < data->number_of_philosophers; i++)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
		free(data);
		return (1);
	}
	for (int i = 0; i < data->number_of_philosophers; i++)
	{
		data->philosophers[i].id = i;
		data->philosophers[i].time_to_die = data->time_to_die;
		data->philosophers[i].time_to_eat = data->time_to_eat;
		data->philosophers[i].time_to_sleep = data->time_to_sleep;
		data->philosophers[i].number_of_meals = data->number_of_meals;
		data->philosophers[i].forks = data->forks;
		data->philosophers[i].print_lock = &data->print_lock;
	}
	for (int i = 0; i < data->number_of_philosophers; i++)
	{
		pthread_mutex_destroy(&data->forks[i]);
	}
	free(data->forks);
	pthread_mutex_destroy(&data->print_lock);
	free(data->philosophers);
	*/
   free(data); 
	return (0);
}
