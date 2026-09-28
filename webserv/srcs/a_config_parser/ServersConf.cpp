/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServersConf.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isadbaib <isadbaib@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 00:05:29 by isadbaib          #+#    #+#             */
/*   Updated: 2026/05/23 00:00:00 by isadbaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <a_config_parser/LocationConf.hpp>
#include <a_config_parser/ServerConf.hpp>
#include <a_config_parser/ServersConf.hpp>

#include <g_utils/Types.hpp>
#include <g_utils/Search.ipp>
#include <g_utils/Logs.hpp>
#include <g_utils/Convertion.hpp>

ServersConf::~ServersConf() {}

ServersConf::ServersConf() : nbServer_(0) {}

const VEC_SERVERS_CONF &ServersConf::getServers() const
{
	return (servers_);
}

const ServerConf &ServersConf::getServer(const SIZET s) const
{
	if (s < 1 || s > nbServer_)
	{
		Logs::fatal("server does not exist: invalid index");
		// static const ServerConf fallback;
		// return (fallback);
	}
	return (servers_[s - 1]);
}

const ServerConf &ServersConf::getServer(const STR &s) const
{
	if (nbServer_ < 1)
	{
		Logs::fatal("no server configured");
		// static const ServerConf fallback;
		// return (fallback);
	}
	int i = earchServer(s);
	if (i < 0)
	{
		Logs::fatal("server not found: " + s);
		// Logs::error("server introuvable: " + s);
		// static const ServerConf fallback;
		// return (fallback);
	}
	return (servers_[i]);
}

int ServersConf::earchServer(const STR &s) const
{
	if (nbServer_ < 1)
	{
		Logs::error("no server configured");
		return (-1);
	}
	SIZET i = 0;
	while (i < servers_.size())
	{
		if (servers_[i].boolNameServer(s))
			return ((int)i);
		i++;
	}
	Logs::error("server name not found: " + s);
	return (-1);
}

SIZET ServersConf::getNbServer() const
{
	return (nbServer_);
}

void ServersConf::createServerVide()
{
	servers_.push_back(ServerConf());
	nbServer_++;
}

ServerConf ServersConf::makeDefaultServer()
{
	VEC_STR tokens;
	tokens.push_back("{");
	tokens.push_back("listen");   tokens.push_back("80");          tokens.push_back(";");
	tokens.push_back("root");     tokens.push_back("www");         tokens.push_back(";");
	tokens.push_back("index");    tokens.push_back("index.html");  tokens.push_back(";");
	tokens.push_back("location"); tokens.push_back("/");
	tokens.push_back("{");
	tokens.push_back("methods");  tokens.push_back("GET");         tokens.push_back(";");
	tokens.push_back("index");    tokens.push_back("index.html");  tokens.push_back(";");
	tokens.push_back("}");
	tokens.push_back("}");

	ServerConf s;
	s.vect_string_config_ = tokens;
	s.create();
	return (s);
}

void ServersConf::createServerValid()
{
	std::set<std::string> seen;
	SIZET i = 0;

	while (i < servers_.size())
	{
		const STR nb = Convertion::toString(i + 1);

		if (!servers_[i].bool_resu_total_ ||
			servers_[i].listeners_.empty()  ||
			servers_[i].getRoot().empty())
		{
			Logs::warn("server " + nb + ": invalid (missing listen or root), replaced by default server");
			servers_[i] = makeDefaultServer();
		}

		const ServerConf &s = servers_[i];

		SET_LISTENERS::const_iterator lit = s.listeners_.begin();
		while (lit != s.listeners_.end())
		{
			const STR portStr = lit->first + ":" + Convertion::toString((SIZET)lit->second);

			if (s.server_name_.empty())
			{
				const STR key = portStr + ":";
				if (seen.count(key))
				{
					Logs::error("server " + nb + ": duplicate listen=" + portStr + " server_name=<empty>");
					servers_[i] = makeDefaultServer();
				}
				else
					seen.insert(key);
			}
			else
			{
				for (SIZET j = 0; j < s.server_name_.size(); j++)
				{
					const STR key = portStr + ":" + s.server_name_[j];
					if (seen.count(key))
					{
						Logs::error("server " + nb + ": duplicate listen=" + portStr
							+ " server_name=" + s.server_name_[j]);
						servers_[i] = makeDefaultServer();
						break;
					}
					seen.insert(key);
				}
			}
			++lit;
		}
		i++;
	}
}


void ServersConf::add(VEC_STR vs)
{
	ServerConf s;
	s.vect_string_config_ = vs;
	s.create();
	servers_.push_back(s);
	nbServer_++;
}

