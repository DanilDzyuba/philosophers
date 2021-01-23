/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clauren <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 17:32:29 by clauren           #+#    #+#             */
/*   Updated: 2021/01/23 17:21:35 by clauren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int			print_s(t_philo *philo, char *msg, int d)
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

int			init_table(t_table *table, int argc, char **argv)
{
	int data[argc];
	int i;

	i = 0;
	if (argc < 5 || argc > 6)
		return (1);
	while (++i < argc)
	{
		data[i] = ft_atoi(argv[i]);
		if (data[i] == -1)
			return (1);
	}
	table->end = 0;
	table->num = data[1];
	table->t_die = data[2];
	table->t_eat = data[3];
	table->t_sleep = data[4];
	table->n_eat = (i == 6) ? data[5] : -1;
	if (!(table->forks = malloc(sizeof(pthread_mutex_t) * table->num)))
		return (1);
	i = -1;
	while (++i < table->num)
		pthread_mutex_init(&table->forks[i], NULL);
	pthread_mutex_init(&table->print, NULL);
	return (0);
}

t_philo		*init_philos(t_table *table)
{
	t_philo	*philos;
	int		i;

	i = -1;
	if (!(philos = malloc(sizeof(t_philo) * table->num)))
		return (NULL);
	while (++i < table->num)
	{
		philos[i].idx = i + 1;
		philos[i].n_eat = 0;
		philos[i].l_fork = i;
		philos[i].r_fork = (i == table->num - 1) ? 0 : i + 1;
		philos[i].start = get_time();
		philos[i].last = philos[i].start;
		philos[i].table = table;
		philos[i].is_hungry = 1;
	}
	return (philos);
}

int			destroy(t_table *table)
{
	int i;

	i = -1;
	while (++i < table->num)
		pthread_mutex_destroy(&table->forks[i]);
	pthread_mutex_destroy(&table->print);
	return (0);
}

void		sleeping(int msec)
{
	long start;

	start = get_time();
	while (get_time() - start < msec)
		usleep(100);
}

int			all_hungry(t_philo *philos)
{
	int i;

	i = -1;
	while (++i < philos[0].table->num)
		if (philos[i].is_hungry)
			return (1);
	return (0);
}

void		*death(void *args)
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

void		*live(void *args)
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

void		dinner(int num, t_philo *philos)
{
	int			i;
	pthread_t	worker[num];
	pthread_t	checker[num];

	i = -1;
	philos[0].table->start = get_time();
	while (++i < num)
	{
		pthread_create(&worker[i], NULL, live, &philos[i]);
		pthread_create(&checker[i], NULL, death, &philos[i]);
		pthread_detach(checker[i]);
		usleep(100);
	}
	i = -1;
	while (!philos[0].table->end)
		if (!all_hungry(philos))
			philos[0].table->end = 1;
	while (++i < num)
	{
		pthread_join(worker[i], NULL);
		pthread_join(checker[i], NULL);
	}
}

int			main(int argc, char **argv)
{
	t_table table;
	t_philo *philos;

	if (init_table(&table, argc, argv))
		return (printf("Input data is invalid\n"));
	if (!(philos = init_philos(&table)))
		return (printf("Error initializing philosophers\n"));
	printf("num: %d | die: %d | eat: %d | sleep: %d | n: %d\n", table.num, table.t_die, table.t_eat, table.t_sleep, table.n_eat);
	dinner(table.num, philos);
	return (destroy(&table));
}
