/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 19:08:56 by rtektas           #+#    #+#             */
/*   Updated: 2025/10/11 19:08:56 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* Vérifie si la simulation est arrêtée */
static int	is_stopped(t_config *cfg)
{
	int	v;

	pthread_mutex_lock(&cfg->state_mutex);
	v = cfg->stopped;
	pthread_mutex_unlock(&cfg->state_mutex);
	return (v);
}

/* Incrémente les compteurs (philo + global) */
static void	mark_meal(t_philo *p)
{
	pthread_mutex_lock(&p->meal_mutex);
	p->meals++;
	pthread_mutex_unlock(&p->meal_mutex);
	pthread_mutex_lock(&p->cfg->state_mutex);
	p->cfg->total_meals++;
	pthread_mutex_unlock(&p->cfg->state_mutex);
}

/* Prend les fourchettes, met à jour, mange puis relâche */
static void	do_eat(t_philo *p)
{
	take_forks(p);
	pthread_mutex_lock(&p->meal_mutex);
	p->last_meal = now_ms();
	pthread_mutex_unlock(&p->meal_mutex);
	print_state(p, EV_EAT);
	mark_meal(p);
	precise_sleep(p->cfg->time_to_eat);
	drop_forks(p);
	usleep(100);
}

/* Dort puis réfléchit */
static void	sleep_then_think(t_philo *p)
{
	print_state(p, EV_SLEEP);
	precise_sleep(p->cfg->time_to_sleep);
	if (is_stopped(p->cfg))
		return ;
	print_state(p, EV_THINK);
	usleep(100);
}

/* Boucle principale du philosophe */
void	*philosopher_routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	pthread_mutex_lock(&p->meal_mutex);
	p->last_meal = now_ms();
	pthread_mutex_unlock(&p->meal_mutex);
	if (p->cfg->count > 1 && (p->id % 2 == 1))
		usleep((p->cfg->time_to_eat * 500));
	if (p->cfg->count == 1)
	{
		pthread_mutex_lock(&p->cfg->forks[p->left]);
		print_state(p, EV_FORK);
		precise_sleep(p->cfg->time_to_die);
		pthread_mutex_unlock(&p->cfg->forks[p->left]);
		return (NULL);
	}
	while (!is_stopped(p->cfg))
	{
		do_eat(p);
		if (is_stopped(p->cfg))
			break ;
		sleep_then_think(p);
		usleep(200);
	}
	return (NULL);
}
