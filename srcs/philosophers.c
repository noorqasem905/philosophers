/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:17:56 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/16 17:52:21 by nqasem           ###   ########.fr       */
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

void print_state(t_philosopher *philo, const char *state)
{
	pthread_mutex_lock(&philo->data->print_lock);
	printf("Philosopher %d %s\n", philo->id, state);
	pthread_mutex_unlock(&philo->data->print_lock);
}
#include <sys/time.h>

long get_time_in_milliseconds(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000L) + (tv.tv_usec / 1000L);
}

int eating(t_philosopher *philo)
{
	pthread_mutex_lock(philo->meal_lock);
	printf(RED "Philosopher %d is eating\n" RESET, philo->id);
	philo->meals_eaten++;
	philo->last_meal = get_time_in_milliseconds();
	pthread_mutex_unlock(philo->meal_lock);
	usleep(philo->time_to_eat * 1000);
	return (0);
}

void thinging(t_philosopher *philo)
{
	pthread_mutex_lock(philo->meal_lock);
	printf(GREEN "Philosopher %d is thinking\n" RESET, philo->id);
	pthread_mutex_unlock(philo->meal_lock);
	usleep(philo->time_to_eat * 1000);
}

void sleeping(t_philosopher *philo)
{
	pthread_mutex_lock(philo->meal_lock);
	printf(BLUE "Philosopher %d is sleeping\n" RESET, philo->id);
	pthread_mutex_unlock(philo->meal_lock);
	usleep(philo->time_to_sleep * 1000);
}


int simulation_has_stopped(t_philosopher *philo)
{
    int stopped;

    pthread_mutex_lock(&philo->data->stop_lock);
    stopped = philo->data->simulation_has_stopped;
    pthread_mutex_unlock(&philo->data->stop_lock);

    return stopped;
}

void	*routine(void *arg)
{
	t_philosopher	*philo = (t_philosopher *)arg;

	// if (philo->id % 2 == 0)
		// usleep(100);
	while (!simulation_has_stopped(philo))
	{
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(&philo->data->forks[philo->forks1[0]]);
			pthread_mutex_lock(&philo->data->forks[philo->forks2[0]]);
		}
		else
		{
			pthread_mutex_lock(&philo->data->forks[philo->forks2[0]]);
			pthread_mutex_lock(&philo->data->forks[philo->forks1[0]]);
		}
		eating(philo);
		pthread_mutex_unlock(&philo->data->forks[philo->forks1[0]]);
		pthread_mutex_unlock(&philo->data->forks[philo->forks2[0]]);
		if (philo->data->number_of_meals != -1 &&
			philo->meals_eaten >= philo->data->number_of_meals)
		{
			printf(GREEN "Philosopher %d is died\n" RESET, philo->id);
			exit(0) ;
		}
		sleeping(philo);
		thinging(philo);
	}
	return (NULL);
}


int	set_philo_data(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->number_of_philosophers)
	{
		data->iteration = i;
		data->philosophers[i].id = i;
		data->philosophers[i].time_to_die = data->time_to_die;
		data->philosophers[i].time_to_eat = data->time_to_eat;
		data->philosophers[i].time_to_sleep = data->time_to_sleep;
		data->philosophers[i].meals_eaten = 0;
		data->philosophers[i].forks1[0] = i;
		data->philosophers[i].forks2[0] = (i + 1) % data->number_of_philosophers;
		data->philosophers[i].forks1[1] = 0;
		data->philosophers[i].forks2[1] = 0;
		data->philosophers[i].meal_lock = &data->print_lock;
		data->philosophers[i].data = data;
		if (pthread_create(&data->philosophers[i].thread, NULL, routine, &data->philosophers[i]) != 0)
		{
			ft_printf("%2Failed to allocate memory for philosophers");
			free(data->philosophers);
			return (-1);
		}
	}
	return (0);
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
	if (set_philo_data(data) < 0)
	{
		ft_printf("%2Failed to set philosopher data");
		free(data->philosophers);
		pthread_mutex_destroy(&data->print_lock);
		while (++i < data->number_of_philosophers)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
		free(data);
		return (-1);
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
