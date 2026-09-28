/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_interpreter.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 20:09:27 by alephoen          #+#    #+#             */
/*   Updated: 2026/07/01 20:09:27 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <c_response/ResponseBuilder.hpp>
#include <d_client/Client.hpp>
#include <g_utils/Types.hpp>
#include <g_utils/utils.hpp>
#include <g_utils/Logs.hpp>
#include <cstring>

BUILD_STATE::STATE		CGIHandler::parse_interpreter(Client &c)
{
	// find extension from request path
	STR::size_type dot = c.request_.path_.rfind('.');
	if (dot == STR::npos)
		return (BUILD_STATE::BUILD_FATAL_ERR);

	STR path_ext = c.request_.path_.substr(dot+1);

	// I removed this so our CGI can handle anything put in the config
	// if (path_ext != "php" && path_ext != "py")
		// return (BUILD_STATE::BUILD_FATAL_ERR);

	// search CGI_locatn_ for matching ext=interpreter
	bool			start_with_dot = false;
	STR::size_type	eq;
	const VEC_STR	&v_cgi		= *c.route_data_.CGI_locatn_;
	SIZET			v_cgi_size	= v_cgi.size();
	SIZET			ext_size;
	SIZET	i = -1;
	while (++i < v_cgi_size)
	{
		if (v_cgi[i].empty()) continue ;

		eq = v_cgi[i].find('=');
		if (eq == STR::npos) continue ;

		start_with_dot =  (v_cgi[i][0] == '.') ? true : false;

		ext_size = eq - start_with_dot;
		if (eq > 0)
		{
			if (strncasecmp(v_cgi[i].c_str()+start_with_dot, path_ext.c_str(), ext_size) == 0)
				{interpreter_ = v_cgi[i].substr(eq+1); break;}
		}
	}
	if (interpreter_.empty()) 
	{
		Logs::warn("CGI NO MATCH ext=" + path_ext + " path=" + c.request_.path_);
		return (BUILD_STATE::BUILD_FATAL_ERR);
	}
	return	BUILD_STATE::BUILD_OK;
}

