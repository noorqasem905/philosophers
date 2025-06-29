/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:17:56 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/29 08:29:50 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!data->simulation_has_stopped)
	{
		i = 0;
		while (i < data->number_of_philosophers)
		{
			pthread_mutex_lock(&data->stop_lock);
			pthread_mutex_lock(&data->philosophers[i].meal_lock);
			if (is_died(data, i) < 0)
				return (NULL);
			pthread_mutex_unlock(&data->stop_lock);
			pthread_mutex_unlock(&data->philosophers[i].meal_lock);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

int	set_philo_data(t_data *data, pthread_t *monitor)
{
	int	i;

	i = -1;
	data->limit_meals = data->number_of_philosophers;
	data->start_t = get_time_in_milliseconds();
	while (++i < data->number_of_philosophers)
	{
		data->philosophers[i].last_meal = 0;
		data->philosophers[i].id = i + 1;
		data->philosophers[i].time_to_die = data->time_to_die;
		data->philosophers[i].time_to_eat = data->time_to_eat;
		data->philosophers[i].time_to_sleep = data->time_to_sleep;
		data->philosophers[i].meals_eaten = 0;
		data->philosophers[i].forks1[0] = i;
		data->philosophers[i].forks2[0] = (i + 1)
			% data->number_of_philosophers;
		data->philosophers[i].data = data;
		pthread_mutex_init(&data->philosophers[i].meal_lock, NULL);
		if (pthread_create(&data->philosophers[i].thread, NULL, routine,
				&data->philosophers[i]) != 0)
			return (-1);
	}
	if (pthread_create(monitor, NULL, monitor_routine, data) != 0)
		return (-1);
	return (0);
}

int	thread_creation(t_data *data, pthread_t *monitor)
{
	int	i;

	i = -1;
	data->philosophers = malloc(sizeof(t_philosopher)
			* data->number_of_philosophers);
	if (!data->philosophers)
	{
		while (++i < data->number_of_philosophers)
			pthread_mutex_destroy(&data->forks[i]);
		return (-1);
	}
	i = -1;
	data->simulation_has_stopped = 0;
	if (set_philo_data(data, monitor) < 0)
	{
		while (++i < data->number_of_philosophers)
		{
			pthread_mutex_destroy(&data->philosophers[i].meal_lock);
			pthread_mutex_destroy(&data->forks[i]);
		}
		return (-1);
	}
	return (0);
}

void	main_handle(t_data *data, int ret)
{
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->meal_limit);
	pthread_mutex_destroy(&data->meal_lock);
	pthread_mutex_destroy(&data->stop_lock);
	if (ret > 0)
		printf("Failed to allocate memory for philosophers\n");
	if (ret >= 0 && (data->limit_meals > 0 || data->number_of_meals == -1))
		printf(RED "%ld Philosopher %d has died\n" RESET,
			get_time_in_milliseconds() - data->start_t, data->id);
	free(data->forks);
	free(data->philosophers);
	free(data);
}

int	main(int argc, char *argv[])
{
	pthread_t	monitor;
	t_data		*data;
	int			ret;
	int			i;

	check_entered_input(argv, argc);
	data = malloc(sizeof(t_data));
	if (!data)
		handle_error_philo(NULL, 1, 5);
	init_data(data);
	if (set_data(data, argc, argv))
		handle_error_philo(data, 1, 2);
	if (setup_mutex_creation(data) < 0)
		return (-1);
	ret = thread_creation(data, &monitor);
	i = -1;
	while (ret >= 0 && ++i < data->number_of_philosophers)
	{
		if (pthread_join(data->philosophers[i].thread, NULL) != 0)
			break ;
	}
	if (ret >= 0)
		pthread_join(monitor, NULL);
	main_handle(data, ret);
	return (0);
}
