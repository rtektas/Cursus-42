/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 19:07:46 by rtektas           #+#    #+#             */
/*   Updated: 2025/10/11 19:07:46 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static const char	*msg(t_event ev)
{
	if (ev == EV_FORK)
		return ("has taken a fork");
	if (ev == EV_EAT)
		return ("is eating");
	if (ev == EV_SLEEP)
		return ("is sleeping");
	if (ev == EV_THINK)
		return ("is thinking");
	return ("died");
}

void	print_state(t_philo *p, t_event ev)
{
	t_ms	ms;
	int		stopped;

	pthread_mutex_lock(&p->cfg->print_mutex);
	ms = now_ms() - p->cfg->start_time;
	pthread_mutex_lock(&p->cfg->state_mutex);
	stopped = p->cfg->stopped;
	pthread_mutex_unlock(&p->cfg->state_mutex);
	if (!stopped || ev == EV_DEAD)
		printf("%lld %d %s\n", ms, p->id + 1, msg(ev));
	pthread_mutex_unlock(&p->cfg->print_mutex);
}
