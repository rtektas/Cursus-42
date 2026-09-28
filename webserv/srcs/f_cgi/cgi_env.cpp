/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_env.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:06:43 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/23 17:06:43 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <f_cgi/CGIHandler.hpp>
#include <c_response/ResponseBuilder.hpp>
#include <d_client/Client.hpp>
#include <g_utils/Types.hpp>
#include <g_utils/utils.hpp>
#include <cctype>

#include <limits.h>
#include <unistd.h>
#include <stdlib.h>

template<typename T>
std::string to_string98(const T& value);

static STR normalize_cgi_key(const STR &str);

/*
It can fail in one case — if c.route_data_.serv_name_ is NULL. 
But we established earlier that this should have been caught during config 
validation, so by the time build_env() is called it should never be NULL.
*/
void	CGIHandler::cgi_env(Client  &c, VEC_STR &envs_strs, VEC_CSTR &envs_ptrs)
{
	const SIZET	ENV_CNT = 21;	// if you grow the env dont forget to grow here!

	STR		str;

	const HEADERS	&headers			= c.request_.headers_;
	const HEADERS	&unknown_headers	= c.request_.unknown_headers_;

	char  cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		exit(1);

	STR	script_filename = 
			STR(cwd) + "/" + *c.route_data_.root_locatn_ + c.request_.path_;

    envs_strs.reserve(ENV_CNT + headers.size() + unknown_headers.size());
    envs_ptrs.reserve(ENV_CNT + headers.size() + unknown_headers.size() + 1);

	envs_strs.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envs_strs.push_back("SERVER_PROTOCOL=HTTP/1.1");
	envs_strs.push_back("SERVER_SOFTWARE=webserv/1.0");
	envs_strs.push_back("REDIRECT_STATUS=1");
	envs_strs.push_back("SERVER_NAME="+*c.route_data_.serv_name_);
	envs_strs.push_back("SERVER_PORT="+to_string98(c.server_port_));
	envs_strs.push_back("REQUEST_METHOD="+c.request_.method_);
	envs_strs.push_back("SCRIPT_NAME="+c.request_.path_);
	envs_strs.push_back("SCRIPT_FILENAME="+script_filename);
	envs_strs.push_back("PATH_INFO="+c.request_.path_);
	envs_strs.push_back("REQUEST_URI="+c.request_.path_);
	envs_strs.push_back("PATH_TRANSLATED=");
	envs_strs.push_back("QUERY_STRING="+c.request_.query_);
	envs_strs.push_back("CONTENT_TYPE="+c.request_.TYPE_);
	envs_strs.push_back("CONTENT_LENGTH="+to_string98(c.request_.body_size_));
	envs_strs.push_back("HTTP_COOKIE=" + c.request_.COOK_);
	envs_strs.push_back("REMOTE_ADDR="+c.ip_);

	SIZET	len = headers.size();
	SIZET	i	= -1;
	while (++i < len)
	{
		if (headers[i].key.empty())
			continue ;

		str = "HTTP_";
		str += normalize_cgi_key(headers[i].key);
		str += "=";
		str += headers[i].val;
		envs_strs.push_back(str);
	}

	len = unknown_headers.size();
	i = -1;
	while (++i < len)
	{
		str = "HTTP_";
		str += normalize_cgi_key(unknown_headers[i].key);
		str += "=";
		str += unknown_headers[i].val;
		envs_strs.push_back(str);
	}

	len = envs_strs.size(); 
	i	= -1;
	while (++i < len)
		envs_ptrs.push_back(const_cast<char*>(envs_strs[i].c_str()));

	envs_ptrs.push_back(NULL);
}

static STR normalize_cgi_key(const STR &str)
{
	STR		key;
	SIZET	len	= str.size();
	SIZET	i	= -1;
	while (++i < len)
	{
		if (str[i] == '-')
			key += '_';
		else
			key += static_cast<char>(std::toupper(static_cast<unsigned char>(str[i])));
	}
	return (key);
}

