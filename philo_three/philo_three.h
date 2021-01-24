/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clauren <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/19 14:01:06 by clauren           #+#    #+#             */
/*   Updated: 2021/01/24 13:30:42 by clauren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_PHILO_THREE_H
# define PHILOSOPHERS_PHILO_THREE_H

# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>
# include <semaphore.h>
# include <signal.h>

typedef struct	s_table
{
	int				num;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				n_eat;
	sem_t			*print;
	sem_t			*forks;
	pid_t			*pids;
	long			start;
	int				end;
}				t_table;

typedef struct	s_philo
{
	int			idx;
	int			n_eat;
	long		start;
	long		last;
	t_table		*table;
}				t_philo;

int				ft_atoi(const char *str);
long			get_time(void);
void			*live(void *args);
void			*death(void *args);
#endif
