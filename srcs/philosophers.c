/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:17:56 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/11 19:22:38 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int		setup_mutex_creation(t_data *data)
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
	return (0);
}

void *test(void *arg)
{
	t_data	*data;
	data = (t_data *)arg;
	// int i = ;
	// while (i < 1000000)
	{// you have to use the mutex to protect the process and set iteration will not be safe eough
	if (data->philosophers->id % 2 == 0)
	{
		pthread_mutex_lock(&data->forks[data->philosophers->forks1[0]]);
		pthread_mutex_lock(&data->forks[data->philosophers->forks2[0]]);
	}
	else
	{
		pthread_mutex_lock(&data->forks[data->philosophers->forks2[0]]);
		pthread_mutex_lock(&data->forks[data->philosophers->forks1[0]]);
	}
	printf("Philosopher %d has taken forks %d and %d\n",
		data->philosophers->id, data->philosophers->forks1[0],
		data->philosophers->forks2[0]);
	// write
	write(1, "Philosopher is eating\n", 22);
	if (data->philosophers->id % 2 == 0)
	{
		pthread_mutex_unlock(&data->forks[data->philosophers->forks1[0]]);
		pthread_mutex_unlock(&data->forks[data->philosophers->forks2[0]]);
	}
	else
	{
		pthread_mutex_unlock(&data->forks[data->philosophers->forks2[0]]);
		pthread_mutex_unlock(&data->forks[data->philosophers->forks1[0]]);
	}
		// inc++;
		// pthread_mutex_unlock(&mutex);
		// i++;
	}
	return (NULL);
}

int thread_creation(t_data *data)
{
	int	i;

	i = -1;
	data->philosophers = malloc(sizeof(t_philosopher) * data->number_of_philosophers);
	if(!data->philosophers)
	{
		ft_printf("%2Failed to allocate memory for philosophers");
		pthread_mutex_destroy(&data->print_lock);
		while (++i < data->number_of_philosophers)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
		free(data);
		return (-1);
	}
	i = -1;
	printf("%d is running\n", data->number_of_philosophers);
	while (++i < data->number_of_philosophers)
	{
		data->iteration = i;
		data->philosophers[i].id = i;
		data->philosophers[i].time_to_die = data->time_to_die;
		data->philosophers[i].time_to_eat = data->time_to_eat;
		data->philosophers[i].time_to_sleep = data->time_to_sleep;
		data->philosophers[i].number_of_meals = data->number_of_meals;
		data->philosophers[i].forks1[0] = i;
		data->philosophers[i].forks2[0] = (i + 1) % data->number_of_philosophers;
		data->philosophers[i].forks1[1] = 0;
		data->philosophers[i].forks2[1] = 0;
		data->philosophers[i].print_lock = &data->print_lock;
		if (pthread_create(&data->philosophers[i].thread, NULL, test, data) != 0)
		{
			ft_printf("%2Failed to allocate memory for philosophers");
			free(data->philosophers);
			return (-1);
		}
	}
	return (0);
}

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
	if (setup_mutex_creation(data) < 0)
		return (-1);
	if (thread_creation(data) < 0)
		return (-1);
	while (data->iteration < data->number_of_philosophers)
	{
		if (pthread_join(data->philosophers[data->iteration].thread, NULL) != 0)
		{
			ft_printf("%2Failed to join thread");
			free(data->philosophers);
			return (-1);
		}
		data->iteration++;
	}
	// pthread_mutex_destroy(&data->print_lock);
   free(data); 
	return (0);
}
