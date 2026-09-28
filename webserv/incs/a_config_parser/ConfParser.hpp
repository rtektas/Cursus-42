/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isadbaib <isadbaib@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 00:15:05 by isadbaib          #+#    #+#             */
/*   Updated: 2026/05/31 01:40:31 by isadbaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONF_PARSER_HPP
#define CONF_PARSER_HPP

#include <a_config_parser/LocationConf.hpp>
#include <a_config_parser/ServersConf.hpp>
#include <g_utils/Types.hpp>

class ConfParser 
{
	private:
	  ServersConf servers_conf_;
	  ConfParser(const ConfParser &s);
	  ConfParser operator=(const ConfParser &s);
	
	  void validaterServers(const ServersConf &servers);
	  void checkNoNestedServer(const VEC_STR &tokens);
	  VEC_STR splitServerBrace(const VEC_STR &tokens);
	public:
	  ~ConfParser();
	  ConfParser();
	
	  ServersConf getServersConf(const STR &conf_path);
  ServersConf getServersConf();
};

#endif

