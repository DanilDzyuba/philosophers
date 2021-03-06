/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clauren <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/24 13:17:26 by clauren           #+#    #+#             */
/*   Updated: 2021/01/24 13:24:03 by clauren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

static	void		sleeping(int msec)
{
	long start;

	start = get_time();
	while (get_time() - start < msec)
		usleep(100);
}

static int			print_s(t_philo *philo, char *msg, int d)
{
	long time;

	pthread_mutex_lock(&philo->table->print);
	if (philo->table->end)
	{
		pthread_mutex_unlock(&philo->table->print);
		return (0);
	}
	time = get_time() - philo->table->start;
	printf("%ld \033[0;36m#%d\033[0m %s\n", time, philo->idx, msg);
	if (d)
		philo->table->end = 1;
	pthread_mutex_unlock(&philo->table->print);
	return (0);
}

void				*death(void *args)
{
	t_philo *philo;
	long	time;
	int		n_eat;

	philo = (t_philo *)args;
	n_eat = (philo->table->n_eat != -1) ? philo->table->n_eat : 0;
	while (!philo->table->end)
	{
		if (n_eat && philo->n_eat == n_eat)
			philo->is_hungry = 0;
		time = get_time() - philo->last;
		if (time > philo->table->t_die && !philo->table->end)
			print_s(philo, "\033[0;31mdied\033[0m", 1);
		usleep(100);
	}
	return (NULL);
}

void				*live(void *args)
{
	t_philo *philo;

	philo = (t_philo *)args;
	while (!philo->table->end)
	{
		pthread_mutex_lock(&philo->table->forks[philo->l_fork]);
		print_s(philo, "has taken a fork", 0);
		pthread_mutex_lock(&philo->table->forks[philo->r_fork]);
		print_s(philo, "has taken a fork", 0);
		philo->last = get_time();
		print_s(philo, "is eating", 0);
		sleeping(philo->table->t_eat);
		philo->n_eat++;
		pthread_mutex_unlock(&philo->table->forks[philo->r_fork]);
		pthread_mutex_unlock(&philo->table->forks[philo->l_fork]);
		print_s(philo, "is sleeping", 0);
		sleeping(philo->table->t_sleep);
		print_s(philo, "is thinking", 0);
	}
	return (NULL);
}

int					all_hungry(t_philo *philos)
{
	int i;

	i = -1;
	while (++i < philos[0].table->num)
		if (philos[i].is_hungry)
			return (1);
	return (0);
}
