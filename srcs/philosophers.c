/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:17:56 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/18 17:12:12 by nqasem           ###   ########.fr       */
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

int	eating(t_philosopher *philo)
{
	if (simulation_has_stopped(philo))
		return (0);
	pthread_mutex_lock(&philo->data->print_lock);
	philo->meals_eaten++;
	if (philo->data->number_of_meals != -1 &&
		philo->meals_eaten >= philo->data->number_of_meals)
		philo->data->limit_meals--;
	philo->last_meal = get_time_in_milliseconds() - philo->data->start_t;
	printf(GREEN "%ld Philosopher %d is eating\n" RESET, philo->last_meal, philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);
	usleep_custom(philo, philo->time_to_eat);
	return (0);
}

void	thinging(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->print_lock);
	printf(GREEN "%ld Philosopher %d is thinking\n" RESET, philo->last_meal, philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);
	usleep_custom(philo, 1);
}

void	sleeping(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->print_lock);
	printf(BLUE "%ld Philosopher %d is sleeping\n" RESET, philo->last_meal, philo->id);
	pthread_mutex_unlock(&philo->data->print_lock);
	usleep_custom(philo, philo->time_to_sleep);
}

void	take_fork(t_philosopher *philo)
{
	if (philo->id % 2 == 0 && philo->data->simulation_has_stopped != 1)
	{
		pthread_mutex_lock(&philo->data->forks[philo->forks1[0]]);
		print_state(philo, "has taken a fork", 9);
		print_state(philo, "has taken a fork", 9);
		pthread_mutex_lock(&philo->data->forks[philo->forks2[0]]);
	}
	else if (philo->data->simulation_has_stopped != 1)
	{
		pthread_mutex_lock(&philo->data->forks[philo->forks2[0]]);
		print_state(philo, "has taken a fork", 9);
		print_state(philo, "has taken a fork", 9);
		pthread_mutex_lock(&philo->data->forks[philo->forks1[0]]);
	}
	eating(philo);
	pthread_mutex_unlock(&philo->data->forks[philo->forks1[0]]);
	pthread_mutex_unlock(&philo->data->forks[philo->forks2[0]]);
}

void	*routine(void *arg)
{
	t_philosopher	*philo = (t_philosopher *)arg;

	if (philo->id % 2 != 0)
		usleep(100);
	while (!simulation_has_stopped(philo))
	{
		take_fork(philo);
		if (philo->data->limit_meals < 0 || philo->last_meal + philo->time_to_die < get_time_in_milliseconds() - philo->data->start_t)
		{
			pthread_mutex_lock(&philo->data->stop_lock);
			philo->data->simulation_has_stopped = 1;
			pthread_mutex_unlock(&philo->data->stop_lock);
			return (NULL);
		}
		if (philo->data->simulation_has_stopped != 1)
			sleeping(philo);
		if (philo->data->simulation_has_stopped != 1)
			thinging(philo);
	}
	return (NULL);
}

int	set_philo_data(t_data *data)
{
	int	i;

	i = -1;
	data->start_t = get_time_in_milliseconds();
	data->limit_meals = data->number_of_meals;
	data->simulation_has_stopped = 0;
	while (++i < data->number_of_philosophers)
	{
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
			printf("Failed to allocate memory for philosophers");
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
		printf("Failed to allocate memory for philosophers");
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
	int i = 0;
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
	if (data->limit_meals > 0)
		printf(RED "%ld Philosopher  has died\n" RESET, get_time_in_milliseconds() - data->start_t);
	// pthread_mutex_destroy(&data->print_lock);
	free(data); 
	return (0);
}
