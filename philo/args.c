/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 19:05:47 by rtektas           #+#    #+#             */
/*   Updated: 2025/10/11 19:05:47 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>

static int	is_number(const char *s)
{
	long long	acc;

	if (!s || !*s)
		return (-1);
	acc = 0;
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return (-1);
		acc = acc * 10 + (*s - '0');
		if (acc > 2147483647)
			return (-1);
		s++;
	}
	return ((int)acc);
}

static int	load_values(int ac, char **av, t_config *cfg)
{
	int	v;

	cfg->count = is_number(av[1]);
	cfg->time_to_die = is_number(av[2]);
	cfg->time_to_eat = is_number(av[3]);
	cfg->time_to_sleep = is_number(av[4]);
	cfg->meals_limit = -1;
	if (ac == 6)
	{
		v = is_number(av[5]);
		if (v <= 0)
			return (-1);
		cfg->meals_limit = v;
	}
	if (cfg->count < 1 || cfg->time_to_die <= 0
		|| cfg->time_to_eat <= 0 || cfg->time_to_sleep <= 0)
		return (-1);
	return (0);
}

/* Analyse et valide les arguments, remplit la config */
int	parse_args(int ac, char **av, t_config *cfg)
{
	if (ac != 5 && ac != 6)
	{
		write(2, "Argument error\n", 15);
		return (-1);
	}
	if (load_values(ac, av, cfg) == -1)
	{
		write(2, "Invalid arguments\n", 18);
		return (-1);
	}
	cfg->total_meals = 0;
	cfg->stopped = 0;
	cfg->forks = NULL;
	return (0);
}
