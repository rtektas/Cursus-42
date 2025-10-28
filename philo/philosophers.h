/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 00:00:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/09/20 00:00:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef long long									t_ms;

typedef struct s_philo								t_philo;
typedef struct s_config								t_config;

typedef enum e_event
{
	EV_FORK,
	EV_EAT,
	EV_SLEEP,
	EV_THINK,
	EV_DEAD
}	t_event;

struct s_config
{
	int						count;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						meals_limit;
	int						total_meals;
	int						stopped;
	t_ms					start_time;
	pthread_mutex_t			print_mutex;
	pthread_mutex_t			state_mutex;
	pthread_mutex_t			*forks;
	t_philo					*philos;
};

struct s_philo
{
	int						id;
	int						left;
	int						right;
	int						meals;
	t_ms					last_meal;
	pthread_mutex_t			meal_mutex;
	pthread_t				thread;
	t_config				*cfg;
};

int							parse_args(int ac, char **av, t_config *cfg);
t_ms						now_ms(void);
void						precise_sleep(t_ms delay);
void						print_state(t_philo *p, t_event ev);
int							init_table(t_config *cfg, t_philo **tab);
void						free_table(t_config *cfg, t_philo **tab);
void						take_forks(t_philo *p);
void						drop_forks(t_philo *p);
void						*philosopher_routine(void *arg);
void						*monitor_routine(void *arg);

#endif
