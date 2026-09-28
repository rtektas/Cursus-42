/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signals.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 18:10:03 by alephoen          #+#    #+#             */
/*   Updated: 2026/04/11 12:26:51 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SIGNALS_HPP
#define SIGNALS_HPP

// #include <signal.h>

namespace Signals
{
	// Initialize signal handlers for the server
	void	init_handlers();

	// Signal handler for SIGINT & SIGTERM
	void	signal_handler(int sig);

	// Check if the server should stop;
	bool	should_stop();

	int		get_stop_sig();
}

#endif

