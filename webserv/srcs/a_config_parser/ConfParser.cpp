/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isadbaib <isadbaib@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 13:11:03 by isadbaib          #+#    #+#             */
/*   Updated: 2026/05/31 11:38:07 by camy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <a_config_parser/ConfParser.hpp>
#include <a_config_parser/ServersConf.hpp>

#include <g_utils/Convertion.hpp>
#include <g_utils/Logs.hpp>
#include <g_utils/ReadFile.hpp>
#include <a_config_parser/Validation.hpp>
#include <g_utils/Types.hpp>

ConfParser::~ConfParser() {}
ConfParser::ConfParser() {}

VEC_STR ConfParser::splitServerBrace(const VEC_STR &tokens)
{
	VEC_STR result;
	SIZET i = 0;

	while (i < tokens.size())
	{
		STR tok = tokens[i];
		while (!tok.empty())
		{
			std::size_t pos = tok.find_first_of("{};");
			if (pos == std::string::npos)
			{
				result.push_back(tok);
				break;
			}
			if (pos > 0)
				result.push_back(tok.substr(0, pos));
			result.push_back(tok.substr(pos, 1));
			tok = tok.substr(pos + 1);
		}
		i++;
	}
	return (result);
}

ServersConf ConfParser::getServersConf(const STR &conf_path)
{
	ReadFile file(conf_path);
	VEC_STR  vStr = splitServerBrace(file.getDataVectorFilter(false));
	ServersConf serv;
	SIZET i = 0;

	while (i < vStr.size())
	{
		if (vStr[i] != STR("server"))
		{
			Logs::fatal("unexpected token outside server block: " + vStr[i]);
			return (serv);
		}

		serv.createServerVide();
		VEC_STR &config = serv.servers_[serv.nbServer_ - 1].vect_string_config_;
		int depth = 0;

		i++;
		if (i >= vStr.size() || vStr[i] != STR("{"))
		{
			Logs::fatal("expected '{' after 'server'");
			return (serv);
		}
		config.push_back(STR("{"));
		depth = 1;
		i++;

		while (i < vStr.size())
		{
			if (vStr[i] == STR("{"))      depth++;
			else if (vStr[i] == STR("}")) depth--;
			config.push_back(vStr[i]);
			i++;
			if (depth == 0) break;
		}

		if (depth != 0)
		{
			Logs::fatal("unclosed server block (missing '}' )");
			return (serv);
		}
		serv.servers_[serv.nbServer_ - 1].create();
	}

	if (serv.nbServer_ < 1)
	{
		Logs::fatal("no server block found in config file");
		return (serv);
	}

	validaterServers(serv);
	serv.createServerValid();
	// clear all vect_string_config_ after validation
	i = -1;
	while (++i < serv.servers_.size())
		serv.servers_[i].vect_string_config_.clear();
	return (serv);
}

ServersConf ConfParser::getServersConf()
{
	ServersConf serv;
	serv.makeDefaultServer();
	serv.nbServer_ = 1;
	return (serv);
}

void ConfParser::checkNoNestedServer(const VEC_STR &tokens)
{
	SIZET i = 0;
	while (i < tokens.size())
	{
		if (tokens[i] == STR("server"))
		{
			Logs::error("keyword 'server' not allowed inside a server block");
			return;
		}
		i++;
	}
}

void ConfParser::validaterServers(const ServersConf &servers)
{
	SIZET i = 0;
	STR   tmp;

	while (++i <= servers.nbServer_)
	{
		const VEC_STR &tokens = servers.getServer(i).getVectStr();
		checkNoNestedServer(tokens);
		tmp = Convertion::getConvertionVectorString(tokens);
		if (!Validation::checkObjet("{}", tmp, 0, 0))
			Logs::error("unbalanced braces in server block");
	}
}

