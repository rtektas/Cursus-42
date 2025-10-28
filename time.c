/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 19:09:12 by rtektas           #+#    #+#             */
/*   Updated: 2025/10/11 19:09:12 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* Renvoie le temps courant en ms */
t_ms	now_ms(void)
{
	struct timeval	v;

	gettimeofday(&v, NULL);
	return ((t_ms)(v.tv_sec * 1000 + v.tv_usec / 1000));
}

/* Dort environ delay ms avec calibration */
void	precise_sleep(t_ms delay)
{
	t_ms	start;
	t_ms	elapsed;

	start = now_ms();
	while (1)
	{
		elapsed = now_ms() - start;
		if (elapsed >= delay)
			break ;
		if (delay - elapsed > 2)
			usleep(50);
		else
			usleep(20);
	}
}
