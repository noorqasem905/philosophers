/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_philo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nqasem <nqasem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:16:13 by nqasem            #+#    #+#             */
/*   Updated: 2025/06/02 16:50:07 by nqasem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	err_mess(int msg)
{
	if (msg == 1)
		ft_printf("%2Error: Invalid number of arguments. Expected 4 or 5 arguments.\n");
	else if (msg == 2)
		ft_printf("%2Error: Invalid argument. All arguments must be valid integers.\n");
	else if (msg == 3)
		ft_printf("%2Error: Number of philosophers must be between 1 and 200.\n");
	else if (msg == 4)
		ft_printf("%2Error: Time values must be greater than or equal to 60.\n");
	else if (msg == 5)
		ft_printf("%2Error: Memory allocation failed.\n");
}

void	*handle_error_philo(t_data *data, int code_err, int meg)
{
	err_mess(meg);
	if (data)
		free(data);
	exit(code_err * -1);
}