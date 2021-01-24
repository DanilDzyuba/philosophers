/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clauren <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 17:32:25 by clauren           #+#    #+#             */
/*   Updated: 2021/01/24 13:21:17 by clauren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

long	get_time(void)
{
	long			time;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (time);
}

int		ft_atoi(const char *str)
{
	long	result;
	long	prev;
	int		sign;

	result = 0;
	while (*str == ' ' || *str == '\r' || *str == '\v' || *str == '\f' ||
			*str == '\t' || *str == '\n')
		str++;
	if (*str == '-' || *str == '+')
		str++;
	if (*(str - 1) == '-')
		sign = -1;
	else
		sign = 1;
	while (*str >= '0' && *str <= '9')
	{
		prev = result;
		result = result * 10 + (*str++ - '0');
		if (result / 10 != prev && sign == -1)
			return (0);
		if (result / 10 != prev && sign == 1)
			return (-1);
	}
	return (*str) ? -1 : (result * sign);
}
