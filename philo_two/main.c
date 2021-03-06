/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clauren <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/17 18:32:09 by clauren           #+#    #+#             */
/*   Updated: 2021/01/24 13:44:10 by clauren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

static	int			init_table(t_table *table, int argc, char **argv)
{
	int data[argc];
	int i;

	i = 0;
	if (argc < 5 || argc > 6)
		return (1);
	while (++i < argc)
	{
		data[i] = ft_atoi(argv[i]);
		if (data[i] <= 0)
			return (1);
	}
	table->end = 0;
	table->num = data[1];
	table->t_die = data[2];
	table->t_eat = data[3];
	table->t_sleep = data[4];
	table->n_eat = (i == 6) ? data[5] : -1;
	return (0);
}

static	int			destroy(t_table *table)
{
	sem_close(table->print);
	sem_close(table->forks);
	sem_unlink("Forks");
	sem_unlink("Print");
	return (0);
}

static	t_philo		*init_philos(t_table *table)
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
		philos[i].start = get_time();
		philos[i].last = philos[i].start;
		philos[i].table = table;
		philos[i].is_hungry = 1;
	}
	destroy(table);
	if ((table->forks =
			sem_open("Forks", O_CREAT, 0600, table->num)) == SEM_FAILED ||
			(table->print = sem_open("Print", O_CREAT, 0600, 1)) == SEM_FAILED)
	{
		printf("Semaphore error\n");
		return (NULL);
	}
	return (philos);
}

static	void		dinner(int num, t_philo *philos)
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

int					main(int argc, char **argv)
{
	t_table table;
	t_philo *philos;

	if (init_table(&table, argc, argv))
		return (printf("Input data is invalid\n"));
	if (!(philos = init_philos(&table)))
		return (printf("Error initializing philosophers\n"));
	printf("num: %d | die: %d | eat: %d | sleep: %d | n: %d\n",
		table.num, table.t_die, table.t_eat, table.t_sleep, table.n_eat);
	dinner(table.num, philos);
	return (destroy(&table));
}
