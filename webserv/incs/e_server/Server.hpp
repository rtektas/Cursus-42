/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 10:16:51 by alephoen          #+#    #+#             */
/*   Updated: 2026/04/20 10:16:51 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVER_HPP
#define	SERVER_HPP

#include <a_config_parser/ServersConf.hpp>
#include <d_client/Client.hpp>
#include <g_utils/Types.hpp>

#include <deque>

class	Server
{
private:
	const RouteConf		&route_conf_;
	MAP_INT_INT			server_fd_port_;

	Server(const Server &s);
	Server&	operator=(const Server &s);

public:
	~Server();
	Server(const RouteConf &route_conf_);

	void	run();
};

#endif

