/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clauren <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/19 14:00:38 by clauren           #+#    #+#             */
/*   Updated: 2021/01/24 13:33:38 by clauren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

static	int			destroy(t_table *table, int kill_all)
{
	int i;

	i = -1;
	if (kill_all)
		while (++i < table->num)
			kill(table->pids[i], SIGKILL);
	sem_close(table->print);
	sem_close(table->forks);
	sem_unlink("Forks");
	sem_unlink("Print");
	return (0);
}

static	int			init_table(t_table *table, int argc, char **argv, int *num)
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
	*num = table->num;
	return (0);
}

static	t_philo		*init_philos(t_table *table)
{
	t_philo	*philos;
	int		i;

	i = -1;
	if (!(philos = malloc(sizeof(t_philo) * table->num)))
		return (NULL);
	if (!(table->pids = malloc(sizeof(pid_t) * table->num)))
		return (NULL);
	while (++i < table->num)
	{
		philos[i].idx = i + 1;
		philos[i].n_eat = 0;
		philos[i].start = get_time();
		philos[i].last = philos[i].start;
		philos[i].table = table;
	}
	destroy(table, 0);
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
	pid_t		child;
	pthread_t	checker[num];

	i = -1;
	philos[0].table->start = get_time();
	while (++i < num)
	{
		child = fork();
		if (child < 0)
			exit(printf("Fork Error"));
		if (child == 0)
		{
			pthread_create(&checker[i], NULL, death, &philos[i]);
			pthread_detach(checker[i]);
			live(&philos[i]);
			exit(11);
		}
		philos[0].table->pids[i] = child;
	}
}

int					main(int argc, char **argv)
{
	t_table table;
	t_philo *philos;
	int		status;
	int		num;

	if (init_table(&table, argc, argv, &num))
		return (printf("Input data is invalid\n"));
	if (!(philos = init_philos(&table)))
		return (printf("Error initializing philosophers\n"));
	printf("num: %d | die: %d | eat: %d | sleep: %d | n: %d\n",
		table.num, table.t_die, table.t_eat, table.t_sleep, table.n_eat);
	dinner(table.num, philos);
	while (1)
	{
		waitpid(-1, &status, 0);
		if (WEXITSTATUS(status) == 11)
		{
			if (num-- && !num)
				break ;
		}
		else
			break ;
	}
	return (destroy(&table, 1));
}
