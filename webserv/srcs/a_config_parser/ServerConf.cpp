/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isadbaib <isadbaib@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 17:43:52 by isadbaib          #+#    #+#             */
/*   Updated: 2026/05/27 00:00:00 by isadbaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <a_config_parser/ServerConf.hpp>
#include <g_utils/Search.ipp>
#include <g_utils/Logs.hpp>
#include <g_utils/Types.hpp>
#include <cstdlib>

// ─── constructeur ──────────────────────────────────────────────────────────

ServerConf::ServerConf()
	: client_max_body_size_((SIZET)-1),
	  autoindex_(false),
	  bool_resu_total_(false)
{}

ServerConf::~ServerConf() { serverClear(); }

void ServerConf::serverClear()
{
	server_name_.clear();
	vect_string_config_.clear();
	listeners_.clear();
	locations_.clear();
}

// ─── setters ───────────────────────────────────────────────────────────────

void ServerConf::setRoot(const STR &v)            { root_ = v; }
void ServerConf::setIndex(const VEC_STR &v)        { index_ = v; }
void ServerConf::setAutoIndex(bool v)              { autoindex_ = v; }
void ServerConf::setClientMaxBodySize(SIZET v)     { client_max_body_size_ = v; }
void ServerConf::addErrorPage(int code, const STR &path) { error_pages_[code] = path; }

// ─── getters ───────────────────────────────────────────────────────────────

const STR&          ServerConf::getRoot()             const { return (root_); }
const VEC_STR&      ServerConf::getIndex()            const { return (index_); }
const MAP_INT_STR&  ServerConf::getErrorPages()       const { return (error_pages_); }
SIZET               ServerConf::getClientMaxBodySize() const { return (client_max_body_size_); }
bool                ServerConf::getAutoIndex()         const { return (autoindex_); }
const VEC_STR&      ServerConf::getServerName()        const { return (server_name_); }
const SET_LISTENERS& ServerConf::getListeners()        const { return (listeners_); }
const VEC_LOCATION_CONF& ServerConf::getLocation()     const { return (locations_); }
const VEC_STR       ServerConf::getVectStr()           const { return (vect_string_config_); }

const STR &ServerConf::getFirstIndex() const
{
	static const STR empty;
	return (index_.empty() ? empty : index_[0]);
}

const PAIR_LISTENER ServerConf::getListen() const
{
	if (listeners_.empty())
		return (PAIR_LISTENER("", 80));
	return (*listeners_.begin());
}

const STR &ServerConf::getPrimaryServerName() const
{
	static const STR empty;
	return (server_name_.empty() ? empty : server_name_[0]);
}

bool ServerConf::boolNameServer(const STR &name) const
{
	return (0 <= earch(server_name_, name));
}

// ─── parseDirective (directives communes a server) ─────────────────────────

bool ServerConf::parseDirective(const VEC_STR &tokens, SIZET &i)
{
	const STR &key = tokens[i];

	if (key == "root")
	{
		i++;
		VEC_STR v = RouteConf::collectUntilSemi(tokens, i, "root");
		if (v.size() != 1) { Logs::fatal("root: one value expected"); return (true); }
		setRoot(v[0]);
		return (true);
	}
	if (key == "index")
	{
		i++;
		setIndex(RouteConf::collectUntilSemi(tokens, i, "index"));
		return (true);
	}
	if (key == "autoindex")
	{
		i++;
		VEC_STR v = RouteConf::collectUntilSemi(tokens, i, "autoindex");
		if (v.size() != 1) { Logs::fatal("autoindex: one value expected"); return (true); }
		const STR valid[] = {"on", "off"};
		if (!RouteConf::isOneOf(v[0], valid)) { Logs::fatal("autoindex: invalid value: " + v[0]); return (true); }
		setAutoIndex(v[0] == "on");
		return (true);
	}
	if (key == "client_max_body_size")
	{
		i++;
		VEC_STR v = RouteConf::collectUntilSemi(tokens, i, "client_max_body_size");
		if (v.size() != 1) { Logs::fatal("client_max_body_size: one value expected"); return (true); }
		setClientMaxBodySize(RouteConf::parseSize(v[0]));
		return (true);
	}
	if (key == "error_page")
	{
		i++;
		VEC_STR v = RouteConf::collectUntilSemi(tokens, i, "error_page");
		if (v.size() != 2) { Logs::fatal("error_page: code + path expected"); return (true); }
		char *endptr;
		long code = std::strtol(v[0].c_str(), &endptr, 10);
		if (*endptr != '\0') { Logs::fatal("error_page: non-numeric code: " + v[0]); return (true); }
		if (code < 400 || code > 599) { Logs::fatal("error_page: code must be 400-599: " + v[0]); return (true); }
		addErrorPage((int)code, v[1]);
		return (true);
	}
	return (false);
}

// ─── parsers specifiques a server ──────────────────────────────────────────

void ServerConf::parseListen(const VEC_STR &tokens, SIZET &i)
{
	i++;
	VEC_STR v = RouteConf::collectUntilSemi(tokens, i, "listen");
	if (v.size() != 1) { Logs::fatal("listen: one value expected"); return; }
	STR   host;
	long  port;
	char  *endptr;
	const STR &val = v[0];
	std::size_t colon = val.find(':');
	if (colon != std::string::npos)
	{
		host = val.substr(0, colon);
		port = std::strtol(val.substr(colon + 1).c_str(), &endptr, 10);
		if (*endptr != '\0') { Logs::fatal("listen: invalid port: " + val); return; }
	}
	else
	{
		host = "0.0.0.0";
		port = std::strtol(val.c_str(), &endptr, 10);
		if (*endptr != '\0') { Logs::fatal("listen: invalid port: " + val); return; }
	}
	if (port <= 0 || port > 65535) { Logs::fatal("listen: port out of range: " + val); return; }
	listeners_.insert(PAIR_LISTENER(host, (int)port));
}

void ServerConf::parseServerName(const VEC_STR &tokens, SIZET &i)
{
	i++;
	VEC_STR v = RouteConf::collectUntilSemi(tokens, i, "server_name");
	server_name_.insert(server_name_.end(), v.begin(), v.end());
}

void ServerConf::parseCharset(const VEC_STR &tokens, SIZET &i)
{
	i++;
	VEC_STR v = RouteConf::collectUntilSemi(tokens, i, "charset");
	if (v.size() != 1) { Logs::error("charset: expected one value"); return; }
	const STR valid[] = {"utf-8", "iso-8859-1", "iso-8859-15", "off"};
	if (!RouteConf::isOneOf(v[0], valid)) { Logs::error("charset: invalid value: " + v[0]); return; }
	charset_ = v[0];
}

void ServerConf::parseEtag(const VEC_STR &tokens, SIZET &i)
{
	i++;
	VEC_STR v = RouteConf::collectUntilSemi(tokens, i, "etag");
	if (v.size() != 1) { Logs::error("etag: expected one value"); return; }
	const STR valid[] = {"on", "off"};
	if (!RouteConf::isOneOf(v[0], valid)) { Logs::error("etag: invalid value: " + v[0]); return; }
	etag_.b = (v[0] == "on");
	etag_.s = v[0];
}

void ServerConf::parseServerTokens(const VEC_STR &tokens, SIZET &i)
{
	i++;
	VEC_STR v = RouteConf::collectUntilSemi(tokens, i, "server_tokens");
	if (v.size() != 1) { Logs::error("server_tokens: expected one value"); return; }
	const STR valid[] = {"on", "off", "build"};
	if (!RouteConf::isOneOf(v[0], valid)) { Logs::error("server_tokens: invalid value: " + v[0]); return; }
	server_tokens_ = v[0];
}

void ServerConf::parseDisableSymlinks(const VEC_STR &tokens, SIZET &i)
{
	i++;
	VEC_STR v = RouteConf::collectUntilSemi(tokens, i, "disable_symlinks");
	if (v.size() != 1) { Logs::error("disable_symlinks: expected one value"); return; }
	const STR valid[] = {"on", "off", "if_not_owner"};
	if (!RouteConf::isOneOf(v[0], valid)) { Logs::error("disable_symlinks: invalid value: " + v[0]); return; }
	disable_symlinks_ = v[0];
}

void ServerConf::parseLocation(const VEC_STR &tokens, SIZET &i)
{
	i++;
	if (i >= tokens.size() || tokens[i] == "{" || tokens[i] == "}" || tokens[i] == ";")
	{
		Logs::error("location: missing path");
		return;
	}
	STR path = tokens[i++];
	if (i >= tokens.size() || tokens[i] != "{")
	{
		Logs::error("location " + path + ": expected '{'");
		return;
	}

	VEC_STR loc_tokens;
	int depth = 0;
	while (i < tokens.size())
	{
		if (tokens[i] == "{") depth++;
		else if (tokens[i] == "}") depth--;
		loc_tokens.push_back(tokens[i++]);
		if (depth == 0) break;
	}
	if (depth != 0) { Logs::error("location " + path + ": unclosed block"); return; }

	LocationConf loc;
	loc.parse(path, loc_tokens);
	loc.ensureDefaultMethods();
	locations_.push_back(loc);
}

void ServerConf::skipDirective(const VEC_STR &tokens, SIZET &i)
{
	Logs::warn("directive inconnue ignoree: " + tokens[i]);
	i++;
	if (i < tokens.size() && tokens[i] == "{")
	{
		int depth = 0;
		while (i < tokens.size())
		{
			if (tokens[i] == "{") depth++;
			else if (tokens[i] == "}") depth--;
			i++;
			if (depth == 0) break;
		}
		return;
	}
	while (i < tokens.size() && tokens[i] != ";" && tokens[i] != "}")
		i++;
	if (i < tokens.size() && tokens[i] == ";")
		i++;
}

// ─── create ────────────────────────────────────────────────────────────────

void ServerConf::create()
{
	SIZET i = 0;

	if (vect_string_config_.empty() || vect_string_config_[0] != "{")
	{
		Logs::fatal("server block: missing opening '{'");
		return;
	}
	i++;

	while (i < vect_string_config_.size())
	{
		const STR &tok = vect_string_config_[i];
		if (tok == "}") break;
		if (tok == ";") { i++; continue; }

		if (parseDirective(vect_string_config_, i)) continue;

		if      (tok == "listen")           parseListen(vect_string_config_, i);
		else if (tok == "server_name")      parseServerName(vect_string_config_, i);
		else if (tok == "charset")          parseCharset(vect_string_config_, i);
		else if (tok == "etag")             parseEtag(vect_string_config_, i);
		else if (tok == "server_tokens")    parseServerTokens(vect_string_config_, i);
		else if (tok == "disable_symlinks") parseDisableSymlinks(vect_string_config_, i);
		else if (tok == "location")         parseLocation(vect_string_config_, i);
		else                                skipDirective(vect_string_config_, i);
	}
	bool_resu_total_ = !listeners_.empty() && !root_.empty();
}

