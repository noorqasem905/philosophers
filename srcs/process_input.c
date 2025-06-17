/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:51:56 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/15 17:12:27 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int check_max_number(char *arv)
{
	int	num;
	int i;

	i = 0;
	num = 0;
	while (arv[i] && (arv[i] == ' ' || (arv[i] >= 9 && arv[i] <= 13)))
		i++;
	if (arv[i] && (arv[i] == '+' || arv[i] == '-'))
		i++;
	while (arv[i])
	{
		num++;
		i++;
	}
	if (num > 10)
		return (-1);
	return (0);
}

int	check_args(int argc, char *argv[])
{
	int	i;
	int	j;

	i = 0;
	if (argc < 5 || argc > 6)
		return (-1);
	while (++i < argc)
	{
		if (check_max_number(argv[i]) < 0)
			return (-2);
		j = 0;
		while (argv[i][j])
		{
			if (!ft_isdigit(argv[i][j]))
				return (-2);
			j++;
		}
	}
	return (0);
}

void	init_data(t_data *data)
{
	if (!data)
		return ;
	data->number_of_philosophers = 0;
	data->time_to_die = 0;
	data->time_to_eat = 0;
	data->time_to_sleep = 0;
	data->number_of_meals = -1;
	data->forks = NULL;
	data->philosophers = NULL;
}

void	check_entered_input(char *arv[], int argc)
{
	long	input;
	int		err_code;

	err_code = check_args(argc, arv);
	if (err_code < 0)
		handle_error_philo(NULL, 1, err_code * -1);
	input = ft_atoi_long(arv[1]);
	if (input == 0 || input > 200 || input < 0)
		handle_error_philo(NULL, 1, 3);
	input = ft_atoi_long(arv[2]);
	if (input == 0 || input < 60 || input > 2147483647)
		handle_error_philo(NULL, 1, 4);
	input = ft_atoi_long(arv[4]);
	if (input == 0 || input < 60 || input > 2147483647)
		handle_error_philo(NULL, 1, 4);
	input = ft_atoi_long(arv[3]);
	if (input == 0 || input < 60 || input > 2147483647)
		handle_error_philo(NULL, 1, 4);
	if (argc == 6 && !ft_atoi_long(arv[5]))
		handle_error_philo(NULL, 1, 2);
}

int	set_data(t_data *data, int argc, char *argv[])
{
	if (!data || !argv)
		return (-1);
	if (check_args(argc, argv) < 0)
		return (-1);
	data->number_of_philosophers = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->number_of_meals = ft_atoi(argv[5]);
	else
		data->number_of_meals = -1;
	return (0);
}
