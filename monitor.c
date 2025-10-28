/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 19:08:34 by rtektas           #+#    #+#             */
/*   Updated: 2025/10/11 19:08:34 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* Marque la simulation arrêtée (thread-safe) */
static void	set_stopped(t_config *cfg)
{
	pthread_mutex_lock(&cfg->state_mutex);
	cfg->stopped = 1;
	pthread_mutex_unlock(&cfg->state_mutex);
}

/* Retourne 1 si l'objectif de repas est atteint */
static int	check_goal(t_config *cfg)
{
	int	achieved;

	achieved = 0;
	if (cfg->meals_limit > 0)
	{
		pthread_mutex_lock(&cfg->state_mutex);
		if (cfg->total_meals >= cfg->meals_limit * cfg->count)
			achieved = 1;
		pthread_mutex_unlock(&cfg->state_mutex);
	}
	return (achieved);
}

/* Retourne l'index d'un mort ou -1 */
static int	find_dead(t_config *cfg, t_ms t)
{
	int		i;
	t_ms	last;

	i = -1;
	while (++i < cfg->count)
	{
		pthread_mutex_lock(&cfg->philos[i].meal_mutex);
		last = cfg->philos[i].last_meal;
		pthread_mutex_unlock(&cfg->philos[i].meal_mutex);
		if (last != 0 && t - last >= (t_ms)cfg->time_to_die)
			return (i);
	}
	return (-1);
}

/* Boucle de surveillance : mort ou fin */
void	*monitor_routine(void *arg)
{
	t_config	*cfg;
	int			dead_id;
	t_ms		now;

	cfg = (t_config *)arg;
	dead_id = -1;
	while (1)
	{
		now = now_ms();
		if (check_goal(cfg))
			break ;
		dead_id = find_dead(cfg, now);
		if (dead_id != -1)
			break ;
		usleep(50);
	}
	set_stopped(cfg);
	if (dead_id != -1)
		print_state(&cfg->philos[dead_id], EV_DEAD);
	return (NULL);
}
