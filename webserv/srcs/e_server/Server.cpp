/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 13:19:57 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/29 13:56:17 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <a_config_parser/ServersConf.hpp>
#include <a_config_parser/RouteConf.hpp>

#include <e_server/Server.hpp>
#include <e_server/ConnectionManager.hpp>

#include <g_utils/Signals.hpp>
#include <g_utils/Logs.hpp>


#include <asm-generic/socket.h>
#include <csignal>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include <stdexcept>

#include <sys/poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <unistd.h>

#include <cerrno>
#include <cstring>

typedef std::runtime_error	runtime_error;

Server::~Server()
{
	MAP_INT_INT::iterator	it		= server_fd_port_.begin();
	MAP_INT_INT::iterator	it_end	= server_fd_port_.end();
	while (it != it_end)
	{ close(it->first); ++it; }
}

template<typename T>
std::string to_string98(const T& value);

int	close_fd(int *fd);

Server::Server(const RouteConf &route_conf_)	: route_conf_(route_conf_)
{
	int	fd;
	int	opt	= 1;
	int	sockopt;

	SET_LISTENERS	listeners = route_conf_.getAllListeners();

	SET_LISTENERS::const_iterator	it		= listeners.begin();
	SET_LISTENERS::const_iterator	it_end	= listeners.end();
	while (it != it_end)
	{
		fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd < 0)
			throw runtime_error("Fatal error: socket failed.");

		sockopt = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		if (sockopt < 0)
			throw runtime_error("Fatal error: setsockopt failed.");

		sockaddr_in		adr;
		adr.sin_family		= AF_INET;
		adr.sin_port		= htons(it->second);
		adr.sin_addr.s_addr	= inet_addr(it->first.c_str());

		if (bind(fd, (sockaddr *) &adr, sizeof(adr)) < 0)
			throw runtime_error
				("Fatal error: " +
				 it->first + ":" + to_string98(it->second) + " bind failed.");

		if (listen(fd, SOMAXCONN) < 0)
			throw runtime_error
				("Fatal error: " +
				 it->first + ":" + to_string98(it->second) + " bind failed.");

		if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
			throw runtime_error("Fatal error: fcntl failed.");

		server_fd_port_[fd] = it->second;

		++it;
	}
}

void	Server::run()
{
	ConnectionManager	con_manager(MAX_ACTIVE_CLIENTS, route_conf_);
	Clients&			clients = con_manager.get_clients();

	std::vector<pollfd>		fds;
	std::map<int, Client*>	cgi_fd_map;

	MAP_INT_INT::iterator server_it;
	MAP_INT_INT::iterator server_fd_it_end = server_fd_port_.end();

	while (!Signals::should_stop())
	{
		// construct the vector for the poll() events from the map
		// it must be reconstructed with each loop so keep it sync.
		cgi_fd_map.clear();
		fds.clear();
		fds.reserve(con_manager.get_clients_cnt() + 1);

		pollfd			p_fd;			// poll_fd

		server_it = server_fd_port_.begin();
		while (server_it != server_fd_it_end)
		{
			p_fd.fd			= server_it->first;
			p_fd.events		= POLLIN;
			p_fd.revents	= 0;
			fds.push_back(p_fd);
			++server_it;
		}

		Clients::const_iterator	const_it = clients.begin();
		while (const_it != clients.end())
		{
			p_fd.fd			= const_it->first;	// client_fd
			
			Client	&client = *(const_it->second);

			if (time(NULL) - client.last_activity_ > CLIENT_TIMEOUT)
			{
				Logs::warn("TIMEOUT fd=" + to_string98(const_it->first) + 
						" elapsed=" + to_string98(time(NULL) - 
							client.last_activity_)     + "s");

				if (client.request_.is_req_ready_ == false)
					{ client.build_error_response(408); }
				int fd = const_it->first;
				++const_it;
				con_manager.rmv_client(fd);
				continue ;
			}

			if (client.cgi_.is_cgi_running())
			{
				BUILD_STATE::STATE	s = client.cgi_.check_timeout(client);
				if (s == BUILD_STATE::BUILD_OK)
					client.respons_.is_ready_ = true;
			}

			p_fd.events		= client.get_poll_events();

			p_fd.revents	= 0;
			fds.push_back(p_fd);

			if (client.cgi_.cgi_ut_fd_ != -1)
			{
				p_fd.fd		= client.cgi_.cgi_ut_fd_;
				p_fd.events	= POLLIN;
				p_fd.revents	= 0;
				fds.push_back(p_fd);
				cgi_fd_map[client.cgi_.cgi_ut_fd_] = &client;
			}

			if (client.cgi_.cgi_in_fd_ != -1)
			{
				p_fd.fd		= client.cgi_.cgi_in_fd_;
				p_fd.events	= POLLOUT;
				p_fd.revents	= 0;
				fds.push_back(p_fd);
				cgi_fd_map[client.cgi_.cgi_in_fd_] = &client;
			}

			++const_it;
		}

		SIZET	fds_size = fds.size();
		int		p = poll(&fds[0], fds_size, 1000);
		if (p < 0)
			break ;

		SIZET	i = -1;
		while (++i < fds_size)
		{
			int		fd = fds[i].fd;

			if (fds[i].revents == 0)
				continue ;

			server_it = server_fd_port_.find(fd);
			if (server_it != server_fd_it_end)
			{
				if (fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
				{
					Logs::error("SERVER SOCKET ERR fd=" + to_string98(fd));
					throw std::runtime_error("server fatal error");
				}
				if (fds[i].revents & POLLIN)
				{
					struct sockaddr_in	client_adr;
					socklen_t			client_adr_len;	//= sizeof(client_adr);

					while (true)
					{
						client_adr_len	= sizeof(client_adr);

						int	client_fd = accept(fd, (struct sockaddr *) &client_adr, &client_adr_len);

						if (client_fd < 0)
						{
							if (errno == EAGAIN || errno == EWOULDBLOCK)
								break ;
							break ;
						}

						STR		client_ip	= inet_ntoa(client_adr.sin_addr);
						int		client_port	= ntohs(client_adr.sin_port);

						fcntl(client_fd, F_SETFL, O_NONBLOCK);

						int nodelay_opt = 1;
						setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &nodelay_opt, sizeof(nodelay_opt));

						Client *clnt = con_manager.add_client(client_fd, client_ip, client_port, server_it->second);
						if (clnt == NULL)
						{	
							Logs::warn("POOL FULL: rejected connection on port " 
									+ to_string98(server_it->second));

							close_fd(&client_fd);
						}
						else
							Logs::info("NEW CONN fd=" + to_string98(client_fd) + 
									" ip=" + client_ip + ":" + 
									to_string98(client_port) + " server_port=" + 
									to_string98(server_it->second));
					}
					continue ;	// client accepted nothing to serve yet...
				}
			}

			std::map<int, Client*>::iterator cgi_it = cgi_fd_map.find(fd);
			if (cgi_it != cgi_fd_map.end())
			{
				Client	&client		= *cgi_it->second;
				int		client_fd	= client.fd_;

				BUILD_STATE::STATE	s;

				if (fds[i].revents & (POLLERR | POLLNVAL))
				{
					client.cgi_.kill_cgi();
					client.build_error_response(500);
				}
				else
				if (fds[i].revents & (POLLIN | POLLHUP))
				{
					s = client.cgi_.on_stdut_ready(client);
					if (s == BUILD_STATE::BUILD_OK)
					{
						client.respons_.is_ready_ = true;
						if (client.request_.CONN_ == true && client.w_buf_.empty())
							{ con_manager.rmv_client(client_fd); continue; }
					}
					if (s == BUILD_STATE::BUILD_FATAL_ERR)
						{ con_manager.rmv_client(client_fd); continue; }
				}
				else
				if (fds[i].revents & POLLOUT)
				{
					s = client.cgi_.on_stdin_ready(client);
					if (s == BUILD_STATE::BUILD_FATAL_ERR)
						{ con_manager.rmv_client(client_fd); continue ;}
				}

				continue ;
			}

			if (fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
				{ con_manager.rmv_client(fd); continue; }

			Clients::iterator it = clients.find(fd);
			if (it != clients.end())
			{
				if (fds[i].revents & POLLIN)
				{
					READ_STATE::STATE	r = it->second->on_read();
					if (r == READ_STATE::READ_CLOSE || 
						// r == READ_STATE::READ_FATAL ||
						r == READ_STATE::READ_ERROR )
						{con_manager.rmv_client(fd); continue; }
					else
					if (r == READ_STATE::READ_DONE || 
						r == READ_STATE::READ_FATAL)
					{
						if (r == READ_STATE::READ_FATAL)
							{int dum = 0; (void) dum;}
						WRIT_STATE::STATE	w = it->second->on_writ();
						if (w == WRIT_STATE::WRIT_FATAL)
							{con_manager.rmv_client(fd); continue; }
						it->second->last_activity_ = time(NULL);
						if (w == WRIT_STATE::WRIT_CGI)
							continue ;
						if (w == WRIT_STATE::WRIT_DONE)
						{
							// handle keep-alive or close
							if (it->second->request_.CONN_ == true)
								{ con_manager.rmv_client(fd); continue; }
							else
							if (!it->second->cgi_.is_cgi_running())
								it->second->keepalive_reset();
							else
							{
								// CGI still running - clear ent bytes, keep connection alive
								it->second->w_buf_.clear();
								it->second->w_pos_ = 0;
							}
						}
					}
					else
						it->second->last_activity_ = time(NULL);
				}
				else
				if (fds[i].revents & POLLOUT)
				{
					WRIT_STATE::STATE	w = it->second->on_writ();
					if (w == WRIT_STATE::WRIT_FATAL)
						{con_manager.rmv_client(fd); continue; }
					if (w == WRIT_STATE::WRIT_CGI)
						continue ;
					it->second->last_activity_ = time(NULL);
					if (w == WRIT_STATE::WRIT_DONE)
					{
						// handle keep-alive or close
						if (it->second->request_.CONN_ == true)
						{ con_manager.rmv_client(fd); continue; }
						else
						if (!it->second->cgi_.is_cgi_running())
							it->second->keepalive_reset();
						else
						{
							// CGI still running - clear ent bytes, keep connection alive
							it->second->w_buf_.clear();
							it->second->w_pos_ = 0;
						}
					}
				}
			}
		}
	}
	if (Signals::get_stop_sig() == SIGINT)
		Logs::warn("Recieved SIGINT (CTRL + C), Time To Exit.");
	
	if (Signals::get_stop_sig() == SIGTERM)
		Logs::warn("Recieved SIGTERM, shutting down.");
	// close all clients before exit
	while (!clients.empty())
		con_manager.rmv_client(clients.begin()->first);
}

