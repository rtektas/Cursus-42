/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 19:53:00 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/01 13:55:10 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include <d_client/Client.hpp>
#include <g_utils/Types.hpp>

class	ConnectionManager
{
private:
	static const SIZET	MAX_ACTIVE_CLIENTS_ = 3000;

	char	buffer_[MAX_ACTIVE_CLIENTS_ * sizeof(Client)];
	Client	*storage_;

	const RouteConf			&route_conf_;

	std::vector<Client*>	free_list_;		// available slots
	Clients					clients_;		// active clients: fd -> client*

	ConnectionManager(const ConnectionManager&);
	ConnectionManager& operator=(const ConnectionManager&);

public:
	~ConnectionManager();
	ConnectionManager(SIZET max_active_clients, const RouteConf	&route_conf);

	Clients&		get_clients();
	const Clients&	get_clients()	const;

	Client*		add_client(int fd, const STR& ip, int port, int server_port);
	void		rmv_client(int fd);
	Client*		get_client(int fd);
	bool		is_client_active(int fd)		const;

	Clients::const_iterator	get_clients_it()	const;

	SIZET		get_clients_cnt()				const;
	SIZET		get_free_list_cnt()				const;

};

#endif

