/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:56:51 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/29 13:45:39 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <c_response/ResponseBuilder.hpp>
#include <d_client/Client.hpp>
#include <g_utils/Types.hpp>
#include <g_utils/utils.hpp>
#include <g_utils/Logs.hpp>

#include <errno.h>

template<typename T>
std::string to_string98(const T& value);

BUILD_STATE::STATE		ResponseBuilder::build_cgi(Client &c)
{
	const STR *root_ptr = c.route_data_.root_locatn_ != NULL ?
						  c.route_data_.root_locatn_ :
						  c.route_data_.root_serv_;

	if (root_ptr == NULL || root_ptr->empty())
		return build_error(c, 500);

	STR	script_path = *root_ptr + c.request_.path_;

	struct stat	script_stat;
	if (stat(script_path.c_str(), &script_stat) == -1)
	{
		if (errno == ENOENT)
		{
			Logs::error("CGI NOT FOUND " + script_path);
			return build_error(c, 500);
		}
		if (errno == EACCES)
		{
			Logs::error("CGI PERMISSION DENIED " + script_path);
			return build_error(c, 403);
		}
		return build_error(c, 500);
	}

	if (S_ISDIR(script_stat.st_mode))
		return build_error(c, 404);
		
	return c.cgi_.cgi_exec(c, script_path);
}

