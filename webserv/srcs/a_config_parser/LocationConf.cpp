/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isadbaib <isadbaib@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 09:00:51 by camy              #+#    #+#             */
/*   Updated: 2026/05/27 00:00:00 by isadbaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <a_config_parser/LocationConf.hpp>
#include <g_utils/Logs.hpp>

#include <cstdlib>
#include <sys/stat.h>

// ─── constructeur ──────────────────────────────────────────────────────────

LocationConf::LocationConf()
	: redirect_code_val_(0),
	  client_max_body_size_((SIZET)-1),
	  autoindex_(false)
{}

LocationConf::~LocationConf() {}

// ─── setters ───────────────────────────────────────────────────────────────

void LocationConf::setRoot(const STR &v)             { root_ = v; }
void LocationConf::setIndex(const VEC_STR &v)         { index_ = v; }
void LocationConf::setMethods(const VEC_STR &v)       { methods_ = v; }
void LocationConf::setCgiExtensions(const VEC_STR &v) { cgi_extensions_ = v; }
void LocationConf::setUploadStore(const STR &v)       { upload_store_ = v; }
void LocationConf::setAutoIndex(bool v)               { autoindex_ = v; }
void LocationConf::setClientMaxBodySize(SIZET v)      { client_max_body_size_ = v; }

void LocationConf::setReturnDirective(const STR &v)
{
	return_directive_  = v;
	redirect_code_val_ = 0;
	redirect_url_val_.clear();
	if (!v.empty())
	{
		std::istringstream iss(v);
		iss >> redirect_code_val_;
		iss >> redirect_url_val_;
	}
}

// ─── getters ───────────────────────────────────────────────────────────────

const STR&     LocationConf::getRoot()              const { return (root_); }
const VEC_STR& LocationConf::getIndex()             const { return (index_); }
const VEC_STR& LocationConf::getMethods()           const { return (methods_); }
const VEC_STR& LocationConf::getCgiExtensions()     const { return (cgi_extensions_); }
const STR&     LocationConf::getReturnDirective()   const { return (return_directive_); }
const STR&     LocationConf::getUploadStore()       const { return (upload_store_); }
SIZET          LocationConf::getClientMaxBodySize() const { return (client_max_body_size_); }
bool           LocationConf::getAutoIndex()         const { return (autoindex_); }
const STR&     LocationConf::getPath()              const { return (path_); }

const STR &LocationConf::getFirstIndex() const
{
	static const STR empty;
	return (index_.empty() ? empty : index_[0]);
}

const int *LocationConf::getRedirectCodePtr() const
{
	return (redirect_code_val_ != 0 ? &redirect_code_val_ : NULL);
}

const STR *LocationConf::getRedirectUrlPtr() const
{
	return (redirect_url_val_.empty() ? NULL : &redirect_url_val_);
}

const VEC_STR &LocationConf::getDirective(const STR &key) const
{
	LOCATION_DIRECTIVES::const_iterator it = directives_.find(key);
	if (it == directives_.end())
	{
		static const VEC_STR empty;
		return (empty);
	}
	return (it->second);
}

// ─── ensureDefaultMethods ──────────────────────────────────────────────────

void LocationConf::ensureDefaultMethods()
{
	if (methods_.empty())
	{
		Logs::warn("location " + path_ + ": no method defined, default: GET");
		methods_.push_back("GET");
	}
}

// ─── parsers specifiques a location ────────────────────────────────────────

void LocationConf::parseMethods(const VEC_STR &tokens, SIZET &i)
{
	i++;
	VEC_STR v = RouteConf::collectUntilSemi(tokens, i, "methods");
	const STR valid[] = {"GET", "POST", "DELETE"};
	for (SIZET j = 0; j < v.size(); j++)
	{
		if (!RouteConf::isOneOf(v[j], valid))
		{
			Logs::error("methods: invalid method: " + v[j]);
			return;
		}
	}
	setMethods(v);
}

void LocationConf::parseReturn(const VEC_STR &tokens, SIZET &i)
{
	i++;
	VEC_STR v = RouteConf::collectUntilSemi(tokens, i, "return");
	if (v.empty() || v.size() > 2)
	{
		Logs::fatal("return: code [url] expected");
		return;
	}
	char *endptr;
	long code = std::strtol(v[0].c_str(), &endptr, 10);
	if (*endptr != '\0') { Logs::fatal("return: non-numeric HTTP code: " + v[0]); return; }
	if (code < 100 || code > 599) { Logs::fatal("return: invalid HTTP code: " + v[0]); return; }
	if (code >= 300 && code < 400 && v.size() != 2)
	{
		Logs::fatal("return: URL required for 3xx redirect");
		return;
	}
	STR ret = v[0];
	if (v.size() == 2) ret += " " + v[1];
	setReturnDirective(ret);
}

// ─── parseDirective (directives communes a location) ───────────────────────

bool LocationConf::parseDirective(const VEC_STR &tokens, SIZET &i)
{
	const STR &key = tokens[i];

	if (key == "root")
	{
		i++;
		VEC_STR v = RouteConf::collectUntilSemi(tokens, i, "root");
		if (v.size() != 1) { Logs::error("root: expected one value"); return (true); }
		setRoot(v[0]);
		return (true);
	}
	if (key == "upload_store")
	{
		i++;
		VEC_STR v = RouteConf::collectUntilSemi(tokens, i, "upload_store");
		if (v.size() != 1) { Logs::error("upload_store: expected one value"); return (true); }
		setUploadStore(v[0]);
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
		if (v.size() != 1) { Logs::error("autoindex: expected one value"); return (true); }
		const STR valid[] = {"on", "off"};
		if (!RouteConf::isOneOf(v[0], valid)) { Logs::error("autoindex: invalid value: " + v[0]); return (true); }
		setAutoIndex(v[0] == "on");
		return (true);
	}
	if (key == "client_max_body_size")
	{
		i++;
		VEC_STR v = RouteConf::collectUntilSemi(tokens, i, "client_max_body_size");
		if (v.size() != 1) { Logs::error("client_max_body_size: expected one value"); return (true); }
		setClientMaxBodySize(RouteConf::parseSize(v[0]));
		return (true);
	}
	if (key == "cgi_handler")
	{
		i++;
		VEC_STR v = RouteConf::collectUntilSemi(tokens, i, "cgi_handler");
		if (v.size() != 2) { Logs::error("cgi_handler: expected extension + interpreter"); return (true); }
		VEC_STR exts = cgi_extensions_;
		exts.push_back(v[0] + "=" + v[1]);
		setCgiExtensions(exts);
		return (true);
	}
	return (false);
}

// ─── parse ─────────────────────────────────────────────────────────────────

void LocationConf::parse(const STR &path, const VEC_STR &tokens)
{
	path_    = path;
	vec_str_ = tokens;
	SIZET i  = 0;

	if (tokens.empty() || tokens[0] != "{")
	{
		Logs::fatal("location " + path + ": missing '{'");
		return;
	}
	i++;

	while (i < tokens.size())
	{
		const STR &tok = tokens[i];
		if (tok == "}") break;
		if (tok == ";") { i++; continue; }

		if (tok == "methods") { parseMethods(tokens, i); continue; }
		if (tok == "return")  { parseReturn(tokens, i);  continue; }

		if (parseDirective(tokens, i)) continue;

		STR key = tokens[i++];
		VEC_STR vals;
		while (i < tokens.size() && tokens[i] != ";" && tokens[i] != "}")
			vals.push_back(tokens[i++]);
		if (i < tokens.size() && tokens[i] == ";")
			i++;
		directives_[key] = vals;
	}
}

