/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 19:42:32 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/02 17:48:09 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	neg;
	int	c;

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
/*
#include <stdio.h>
int main(){
printf("%d",ft_atoi("  -154236ab"));
}*/
