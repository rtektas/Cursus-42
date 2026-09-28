/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 13:47:17 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/01 13:49:35 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <e_server/ConnectionManager.hpp>
#include <g_utils/Types.hpp>

ConnectionManager::~ConnectionManager()
{
	SIZET i = -1;
	while (++i < MAX_ACTIVE_CLIENTS_)
		storage_[i].~Client();
}

ConnectionManager::ConnectionManager
	(SIZET max_active_clients, const RouteConf	&route_conf)
				: storage_(reinterpret_cast<Client*>(buffer_)),
				  route_conf_(route_conf)
{
	(void) max_active_clients;

	SIZET i = -1;

	while (++i < MAX_ACTIVE_CLIENTS_)
	{
		new (&storage_[i]) Client(route_conf_);
	}

	i = -1;
	while (++i < MAX_ACTIVE_CLIENTS_)
		free_list_.push_back(&storage_[i]);
}

Client*		
ConnectionManager::add_client(int fd, const STR& ip, int port, int server_port)
{
	if (free_list_.empty())
		return (NULL);			// pool exhausted

	Client		*client = free_list_.back();

	if (client->fd_ != -1)		// This fd hasnt been freed yet!
		return (NULL);

	free_list_.pop_back();

	client->activate(fd, ip, port, server_port);
	clients_[fd] = client;

	return (client);
}

void		ConnectionManager::rmv_client(int fd)
{
	Clients::iterator	it = clients_.find(fd);
	if (it == clients_.end())
		return ;

	Client *client = it->second;
	clients_.erase(it);				

	client->deactivate();
	free_list_.push_back(client);
}

Clients&	ConnectionManager::get_clients() { return clients_; }

const Clients&		ConnectionManager::get_clients() const	{ return clients_; }

Client*		ConnectionManager::get_client(int fd)
{
	Clients::iterator	it = clients_.find(fd);
	if (it == clients_.end())
		return (NULL);

	return (it->second);
}

SIZET		ConnectionManager::get_clients_cnt()	const
{
	return clients_.size();
}

SIZET		ConnectionManager::get_free_list_cnt()	const
{
	return free_list_.size();
}

