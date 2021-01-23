/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clauren <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/19 14:00:38 by clauren           #+#    #+#             */
/*   Updated: 2021/01/24 01:42:54 by clauren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int			destroy(t_table *table, int kill_all)
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

int			print_s(t_philo *philo, char *msg, int d)
{
	long time;

	sem_wait(philo->table->print);
	if (philo->table->end)
	{
		sem_post(philo->table->print);
		return (0);
	}
	if (d)
		philo->table->end = 1;
	time = get_time() - philo->table->start;
	printf("%ld \033[0;36m#%d\033[0m %s\n", time, philo->idx, msg);
	if (!d)
		sem_post(philo->table->print);
	return (0);
}

int			init_table(t_table *table, int argc, char **argv, int *num)
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
	*num = table->num;
	return (0);
}

t_philo		*init_philos(t_table *table)
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

void		sleeping(int msec)
{
	long start;

	start = get_time();
	while ((get_time() - start) < msec)
		usleep(100);
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
			exit(11);
		time = get_time() - philo->last;
		if (time > philo->table->t_die && !philo->table->end)
		{
			print_s(philo, "\033[0;31mdied\033[0m", 1);
			exit(22);
		}
		usleep(10);
	}
	return (NULL);
}

void		*live(void *args)
{
	t_philo *philo;

	philo = (t_philo *)args;
	while (!philo->table->end)
	{
		sem_wait(philo->table->forks);
		print_s(philo, "has taken a fork", 0);
		sem_wait(philo->table->forks);
		print_s(philo, "has taken a fork", 0);
		philo->last = get_time();
		print_s(philo, "is eating", 0);
		sleeping(philo->table->t_eat);
		sem_post(philo->table->forks);
		sem_post(philo->table->forks);
		philo->n_eat++;
		print_s(philo, "is sleeping", 0);
		sleeping(philo->table->t_sleep);
		print_s(philo, "is thinking", 0);
	}
}

void		dinner(int num, t_philo *philos)
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

int			main(int argc, char **argv)
{
	t_table table;
	t_philo *philos;
	int 	status;
	int 	num;

	if (init_table(&table, argc, argv, &num))
		return (printf("Input data is invalid\n"));
	if (!(philos = init_philos(&table)))
		return (printf("Error initializing philosophers\n"));
	printf("num: %d | die: %d | eat: %d | sleep: %d | n: %d\n", table.num, table.t_die, table.t_eat, table.t_sleep, table.n_eat);
	dinner(table.num, philos);
	while (1)
	{
		waitpid(-1, &status, 0);
		if (WEXITSTATUS(status) == 11)
		{
			if (num-- && !num)
				break;
		}
		else
			break;
	}
	return (destroy(&table, 1));
}