/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 00:00:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/09/20 00:00:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	assign_forks(t_philo *p, int i, int count)
{
	p->id = i;
	if (i % 2 == 0)
	{
		p->left = i;
		p->right = (i + 1) % count;
	}
	else
	{
		p->right = i;
		p->left = (i - 1 + count) % count;
	}
}

static int	alloc_arrays(t_config *cfg, t_philo **tab)
{
	*tab = malloc(sizeof(t_philo) * cfg->count);
	if (!*tab)
		return (-1);
	cfg->forks = malloc(sizeof(pthread_mutex_t) * cfg->count);
	if (!cfg->forks)
	{
		free(*tab);
		*tab = NULL;
		return (-1);
	}
	return (0);
}

static void	init_mutexes(t_config *cfg)
{
	int	i;

	i = 0;
	while (i < cfg->count)
	{
		pthread_mutex_init(&cfg->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&cfg->print_mutex, NULL);
	pthread_mutex_init(&cfg->state_mutex, NULL);
}

static void	init_philos(t_config *cfg, t_philo *tab)
{
	int	i;

	i = 0;
	while (i < cfg->count)
	{
		assign_forks(&tab[i], i, cfg->count);
		tab[i].meals = 0;
		tab[i].last_meal = 0;
		tab[i].cfg = cfg;
		pthread_mutex_init(&tab[i].meal_mutex, NULL);
		i++;
	}
}

int	init_table(t_config *cfg, t_philo **tab)
{
	if (alloc_arrays(cfg, tab) == -1)
		return (-1);
	init_mutexes(cfg);
	init_philos(cfg, *tab);
	cfg->philos = *tab;
	return (0);
}
