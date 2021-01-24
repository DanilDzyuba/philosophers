/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clauren <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 17:32:51 by clauren           #+#    #+#             */
/*   Updated: 2021/01/24 13:23:20 by clauren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_PHILO_ONE_H
# define PHILOSOPHERS_PHILO_ONE_H

# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>

typedef struct	s_table
{
	int				num;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				n_eat;
	pthread_mutex_t	print;
	pthread_mutex_t	*forks;
	long			start;
	int				end;
}				t_table;

typedef struct	s_philo
{
	int			idx;
	int			is_hungry;
	int			l_fork;
	int			r_fork;
	int			n_eat;
	long		start;
	long		last;
	t_table		*table;
}				t_philo;

int				ft_atoi(const char *str);
long			get_time(void);
int				all_hungry(t_philo *philos);
void			*live(void *args);
void			*death(void *args);
#endif
