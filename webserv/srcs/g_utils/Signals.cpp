/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signals.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 18:14:59 by alephoen          #+#    #+#             */
/*   Updated: 2026/04/04 18:55:23 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <b_request/HTTParser.hpp>
#include <g_utils/Signals.hpp>
#include <g_utils/Logs.hpp>

#include <csignal>

namespace
{
	volatile sig_atomic_t	stop		= 0;
	volatile sig_atomic_t	stop_sig	= 0;

	void	set_stop(int sig)
	{
		stop = 1;
		stop_sig = sig;
	}
}

namespace Signals
{
	void	init_handlers()
	{
		struct sigaction	sa;
		sa.sa_handler = signal_handler;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;

		// SIGINT (ctrl + c)
		if (sigaction(SIGINT, &sa, NULL) == -1)
			Logs::warn("Error: SIGINT failed.");

		// SIGTERM
		if (sigaction(SIGTERM, &sa, NULL) == -1)
			Logs::warn("Error: SIGTERM failed.");

		// SIGUSR1 — dump in-memory debug log on demand
		// sa.sa_handler = dump_log_handler;		// this for the debug log
		if (sigaction(SIGUSR1, &sa, NULL) == -1)
			Logs::warn("Error: SIGUSR1 failed.");
	}

	void	signal_handler(int sig)
	{
		if (sig == SIGINT)
		{
			// Log::warn("Recieved SIGINT (CTRL + C), Time To Exit.");
			set_stop(sig);
		}
		else if (sig == SIGTERM)
		{
			// Log::warn("Recieved SIGTERM, shutting down.");
			set_stop(sig);
		}
	}

	bool	should_stop()
	{
		return (stop);
	}

	int		get_stop_sig()
	{
		return (stop_sig);
	}
}

