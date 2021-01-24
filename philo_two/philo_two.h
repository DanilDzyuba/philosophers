/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clauren <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 17:32:51 by clauren           #+#    #+#             */
/*   Updated: 2021/01/24 13:39:09 by clauren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_PHILO_TWO_H
# define PHILOSOPHERS_PHILO_TWO_H

# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>
# include <semaphore.h>

typedef struct	s_table
{
	int				num;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				n_eat;
	sem_t			*print;
	sem_t			*forks;
	long			start;
	int				end;
}				t_table;

typedef struct	s_philo
{
	int			idx;
	int			n_eat;
	int			is_hungry;
	long		start;
	long		last;
	t_table		*table;
}				t_philo;

int				ft_atoi(const char *str);
long			get_time(void);
void			*live(void *args);
void			*death(void *args);
int				all_hungry(t_philo *philos);
#endif
