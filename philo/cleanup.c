/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 00:00:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/10/11 00:00:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_table(t_config *cfg, t_philo **tab)
{
	int	idx;

	idx = -1;
	while (++idx < cfg->count)
		pthread_mutex_destroy(&(*tab)[idx].meal_mutex);
	if (cfg->forks)
	{
		idx = -1;
		while (++idx < cfg->count)
			pthread_mutex_destroy(&cfg->forks[idx]);
		free(cfg->forks);
		cfg->forks = NULL;
	}
	pthread_mutex_destroy(&cfg->print_mutex);
	pthread_mutex_destroy(&cfg->state_mutex);
	if (*tab)
	{
		free(*tab);
		*tab = NULL;
	}
}
