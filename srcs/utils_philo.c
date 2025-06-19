/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:23:37 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/18 16:25:07 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
	{
		return (1);
	}
	return (0);
}

long	ft_atoi_long(const char *str)
{
	int		neg;
	int		i;
	long	c;

	c = 0;
	i = 0;
	neg = 1;
	while (str[i] && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] && (str[i] == '+' || str[i] == '-'))
	{
		if (str[i] == '-')
			neg *= -1;
		i++;
	}
	while (str[i] && str[i] <= '9' && str[i] >= '0')
	{
		c = (c * 10) + (str[i] - '0');
		i++;
	}
	return (c * neg);
}

int	ft_atoi(const char *str)
{
	int i;
	int neg;
	int c;

	c = 0;
	i = 0;
	neg = 1;
	while (str[i] && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] && (str[i] == '+' || str[i] == '-'))
	{
		if (str[i] == '-')
			neg *= -1;
		i++;
	}
	while (str[i] && str[i] <= '9' && str[i] >= '0')
	{
		c = (c * 10) + (str[i] - '0');
		i++;
	}
	return (c * neg);
}