/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clauren <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 17:32:29 by clauren           #+#    #+#             */
/*   Updated: 2021/01/10 21:20:04 by clauren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int init_table(t_table *table, int argc, char **argv)
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
	table->num = data[1];
	table->t_die = data[2];
	table->t_eat = data[3];
	table->t_sleep = data[4];
	table->n_eat = (i == 6) ? data[5] : -1;
	return (0);
}

int main(int argc, char **argv)
{
	t_table table;

	if (init_table(&table, argc, argv))
		return (printf("Input data is invalid\n"));
	printf("num: %d | die: %d | eat: %d | sleep: %d | n: %d", table.num, table.t_die, table.t_eat, table.t_sleep, table.n_eat);
	long start = get_time();
	while (1)
	{
		printf("\n%ld", get_time() - start);
		usleep(1000);
	}
	return 0;
}
