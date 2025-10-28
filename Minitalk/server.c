/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 18:49:35 by rtektas           #+#    #+#             */
/*   Updated: 2024/11/24 21:42:47 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	ft_decrypte(int sig)
{
	static char	str = 0;
	static int	get_byte = 0;

	if (sig == SIGUSR1)
		str = str | 1;
	if (++get_byte == 8)
	{
		get_byte = 0;
		if (!str)
			ft_printf("\n");
		ft_printf("%c", str);
		str = 0;
	}
	else
		str <<= 1;
}

int	main(void)
{
	ft_printf("process_id => %d\n", getpid());
	signal(SIGUSR1, ft_decrypte);
	signal(SIGUSR2, ft_decrypte);
	while (1)
	{
		pause();
	}
	return (0);
}
