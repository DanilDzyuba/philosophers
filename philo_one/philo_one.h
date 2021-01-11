/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clauren <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 17:32:51 by clauren           #+#    #+#             */
/*   Updated: 2021/01/10 20:57:24 by clauren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_PHILO_ONE_H
# define PHILOSOPHERS_PHILO_ONE_H

# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct	s_philo
{
	int			idx;
	int 		is_hungry;
	int			l_fork;
	int 		r_fork;
	int			n_eat;
	long 		start;
	long 		last;
}				t_philo;

typedef struct	s_table
{
	int 		num;
	int 		t_die;
	int 		t_eat;
	int 		t_sleep;
	int 		n_eat;
}				t_table;

int		ft_strlen(char *s);
int		ft_atoi(const char *str); // TODO CHECK FOR LONG NUMBER
int		print(char *s);
long	get_time(void);

#endif
