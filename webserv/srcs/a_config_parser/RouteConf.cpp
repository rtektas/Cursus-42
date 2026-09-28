/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConf.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isadbaib <isadbaib@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 00:00:00 by isadbaib          #+#    #+#             */
/*   Updated: 2026/05/27 00:00:00 by isadbaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <a_config_parser/RouteConf.hpp>
#include <a_config_parser/ServersConf.hpp>
#include <a_config_parser/LocationConf.hpp>
#include <g_utils/Logs.hpp>

#include <cstdlib>
#include <sys/stat.h>

// ─── constructeur ──────────────────────────────────────────────────────────

static ServersConf &_dummy()
{
	static ServersConf d;
	return (d);
}

RouteConf::RouteConf()             : servers_conf_(_dummy()) {}
RouteConf::RouteConf(ServersConf &sc) : servers_conf_(sc) {}
RouteConf::~RouteConf() {}

// ─── helpers statiques ─────────────────────────────────────────────────────

VEC_STR RouteConf::collectUntilSemi(const VEC_STR &tokens, SIZET &i, const STR &ctx)
{
	VEC_STR vals;
	if (i >= tokens.size() || tokens[i] == ";" || tokens[i] == "}")
	{
		Logs::error(ctx + ": missing value");
		return (vals);
	}
	while (i < tokens.size() && tokens[i] != ";" && tokens[i] != "}")
		vals.push_back(tokens[i++]);
	if (i >= tokens.size() || tokens[i] != ";")
	{
		Logs::error(ctx + ": missing ';'");
		return (vals);
	}
	i++;
	return (vals);
}

SIZET RouteConf::parseSize(const STR &val)
{
	static const SIZET DEFAULT_SIZE = (SIZET)1 * 1024 * 1024;
	if (val.empty())
	{
		Logs::error("client_max_body_size: empty value, default 1M applied");
		return (DEFAULT_SIZE);
	}
	char  last = val[val.size() - 1];
	SIZET num;
	if (last == 'm' || last == 'M' || last == 'k' || last == 'K' || last == 'g' || last == 'G')
		num = (SIZET)std::atol(val.substr(0, val.size() - 1).c_str());
	else
		num = (SIZET)std::atol(val.c_str());
	if (last == 'm' || last == 'M') return (num * (SIZET)1024 * 1024);
	if (last == 'k' || last == 'K') return (num * (SIZET)1024);
	if (last == 'g' || last == 'G') return (num * (SIZET)1024 * 1024 * 1024);
	return (num);
}

// ─── getRouteData ──────────────────────────────────────────────────────────

const RouteData RouteConf::getRouteData(int port, const STR &host_name, const STR &path) const
{
	const ServerConf *default_server = NULL;
	const ServerConf *matched_server = NULL;
	SIZET i;
	const VEC_SERVERS_CONF &servers = servers_conf_.getServers();

	// Step 1 : trouver le bloc server
	i = 0;
	while (i < servers.size())
	{
		bool port_match = false;
		const SET_LISTENERS &lsts = servers[i].getListeners();
		SET_LISTENERS::const_iterator it = lsts.begin();
		while (it != lsts.end())
		{
			if (it->second == port) { port_match = true; break; }
			++it;
		}
		if (port_match)
		{
			if (default_server == NULL)
				default_server = &servers[i];
			if (servers[i].boolNameServer(host_name))
			{
				matched_server = &servers[i];
				break;
			}
		}
		++i;
	}

	if (matched_server == NULL && default_server == NULL)
		return (RouteData());
	if (matched_server == NULL)
		matched_server = default_server;

	// Step 2 : trouver la location (plus long prefixe + boundary)
	const LocationConf *best_match       = NULL;
	SIZET               best_match_length = 0;
	const VEC_LOCATION_CONF &locs = matched_server->getLocation();

	i = 0;
	while (i < locs.size())
	{
		const STR &route = locs[i].getPath();
		if (path.rfind(route, 0) == 0)
		{
			if (path.size() == route.size() ||
				path[route.size()] == '/' ||
				(route.size() > 0 && route[route.size() - 1] == '/'))
			{
				if (route.size() >= best_match_length)
				{
					best_match        = &locs[i];
					best_match_length = route.size();
				}
			}
		}
		++i;
	}

	if (best_match == NULL)
		return (RouteData());

	// Step 3 : remplir RouteData
	static const STR   default_index("index.html");
	// static const SIZET default_cmbs = 1048576;	// 1mb is not enough!
	static const SIZET default_cmbs = MAX_BODY_SIZE;

	SIZET cmbs     = matched_server->getClientMaxBodySize();
	if (cmbs == (SIZET)-1) cmbs = default_cmbs;
	SIZET loc_cmbs = best_match->getClientMaxBodySize();
	SIZET cmbl     = (loc_cmbs != (SIZET)-1) ? loc_cmbs : cmbs;

	RouteData result;
	result.client_max_body_size_serv_   = cmbs;
	result.autoindex_                   = best_match->getAutoIndex();
	result.client_max_body_size_locatn_ = cmbl;

	result.serv_name_ = &matched_server->getPrimaryServerName();
	result.listeners_ = &matched_server->getListeners();
	result.root_serv_ = &matched_server->getRoot();
	result.eror_page_ = &matched_server->getErrorPages();
	result.idex_serv_ = matched_server->getFirstIndex().empty()
		? &default_index : &matched_server->getFirstIndex();

	result.rout_locatn_ = &best_match->getPath();
	result.meth_locatn_ = &best_match->getMethods();
	result.uplod_store_ = best_match->getUploadStore().empty()
		? NULL : &best_match->getUploadStore();
	result.CGI_locatn_  = best_match->getCgiExtensions().empty()
		? NULL : &best_match->getCgiExtensions();
	result.redirect_code_ = best_match->getRedirectCodePtr();
	result.redirect_url_  = best_match->getRedirectUrlPtr();
	result.root_locatn_ = best_match->getRoot().empty()
		? &matched_server->getRoot() : &best_match->getRoot();
	result.idex_locatn_ = !best_match->getFirstIndex().empty()
		? &best_match->getFirstIndex() : result.idex_serv_;

	return (result);
}

SET_LISTENERS	RouteConf::getAllListeners()	const
{
	SET_LISTENERS	result;

	const VEC_SERVERS_CONF	&servers		= servers_conf_.getServers();

	VEC_SERVERS_CONF::const_iterator	servers_it	= servers.begin();
	VEC_SERVERS_CONF::const_iterator	servers_end = servers.end();

	while (servers_it != servers_end)
	{
		const SET_LISTENERS &listeners = servers_it->getListeners();

		SET_LISTENERS::const_iterator	listeners_it	= listeners.begin();
		SET_LISTENERS::const_iterator listeners_end	= listeners.end();
		while (listeners_it != listeners_end)
		{
			result.insert(*listeners_it);
			++listeners_it;
		}
		++servers_it;
	}

	return (result);
}

