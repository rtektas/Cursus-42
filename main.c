/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 00:00:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/09/20 00:00:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* Lance les threads en deux passes pour étaler le départ */
static void	launch_philosophers(t_config *cfg, t_philo *tab)
{
	int	i;

	i = 0;
	while (i < cfg->count)
	{
		pthread_create(&tab[i].thread, NULL, philosopher_routine, &tab[i]);
		usleep(100);
		i += 2;
	}
	i = 1;
	while (i < cfg->count)
	{
		pthread_create(&tab[i].thread, NULL, philosopher_routine, &tab[i]);
		usleep(100);
		i += 2;
	}
}

/* Traite les arguments, alloue la table, initialise le temps */
static int	initialize(int ac, char **av, t_config *cfg, t_philo **table)
{
	int	i;

	if (parse_args(ac, av, cfg) == -1)
		return (-1);
	if (init_table(cfg, table) == -1)
		return (-1);
	cfg->start_time = now_ms();
	i = -1;
	while (++i < cfg->count)
		(*table)[i].last_meal = cfg->start_time;
	return (0);
}

/* Lance la simulation puis attend la fin */
/* Point d'entrée : init, lancement, attente, nettoyage */
int	main(int ac, char **av)
{
	t_config	cfg;
	t_philo		*table;
	pthread_t	monitor;
	int			i;

	if (initialize(ac, av, &cfg, &table) == -1)
		return (1);
	if (pthread_create(&monitor, NULL, monitor_routine, &cfg) != 0)
	{
		free_table(&cfg, &table);
		return (1);
	}
	launch_philosophers(&cfg, table);
	i = -1;
	while (++i < cfg.count)
		pthread_join(table[i].thread, NULL);
	pthread_join(monitor, NULL);
	free_table(&cfg, &table);
	return (0);
}
