/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServersConf.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isadbaib <isadbaib@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 11:30:38 by isadbaib          #+#    #+#             */
/*   Updated: 2026/05/31 11:30:38 by isadbaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERS_CONF_HPP
#define SERVERS_CONF_HPP

#include <a_config_parser/ServerConf.hpp>
#include <g_utils/Types.hpp>

class	ServersConf
{
private:
	VEC_SERVERS_CONF servers_;
	SIZET nbServer_;
	int earchServer(const STR &s) const;
	void createServerValid();
	// static ServerConf makeDefaultServer();
	friend class ConfParser;


public:
	~ServersConf();
	ServersConf();

	const VEC_SERVERS_CONF &getServers() const;
	const ServerConf &getServer(const SIZET s) const;
	const ServerConf &getServer(const STR &s) const;
	void add(VEC_STR vs);
	void createServerVide();
	SIZET getNbServer() const;

	ServerConf makeDefaultServer();
};

#endif

