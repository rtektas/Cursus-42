/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 19:07:07 by rtektas           #+#    #+#             */
/*   Updated: 2025/10/11 19:20:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* Prend les fourchettes selon un ordre global pour éviter le deadlock */
void	take_forks(t_philo *p)
{
	int	first;
	int	second;

	if (p->left < p->right)
	{
		first = p->left;
		second = p->right;
	}
	else
	{
		first = p->right;
		second = p->left;
	}
	pthread_mutex_lock(&p->cfg->forks[first]);
	print_state(p, EV_FORK);
	pthread_mutex_lock(&p->cfg->forks[second]);
	print_state(p, EV_FORK);
}

/* Relâche les fourchettes (ordre inverse) */
void	drop_forks(t_philo *p)
{
	int	first;
	int	second;

	if (p->left < p->right)
	{
		first = p->left;
		second = p->right;
	}
	else
	{
		first = p->right;
		second = p->left;
	}
	pthread_mutex_unlock(&p->cfg->forks[second]);
	pthread_mutex_unlock(&p->cfg->forks[first]);
}
