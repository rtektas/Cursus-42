/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_delete.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:56:41 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/29 13:42:24 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <c_response/ResponseBuilder.hpp>
#include <d_client/Client.hpp>
#include <g_utils/Logs.hpp>

#include <cerrno>
#include <fcntl.h>

template<typename T>
std::string to_string98(const T& value);

int	close_fd(int *fd);

BUILD_STATE::STATE	ResponseBuilder::build_delete(Client &c)
{
	const RouteData	&route_data = c.route_data_;

	const STR	*root_ptr = route_data.root_locatn_	!= NULL ?
							route_data.root_locatn_ :
							route_data.root_serv_	;
	if (root_ptr == NULL || root_ptr->empty())
		return build_error(c, 500);
	const STR	&root = *root_ptr;

	c.respons_.full_path_	= root + c.request_.path_;
	STR	&full_path			= c.respons_.full_path_;

	c.respons_.fd_ = open(full_path.c_str(), O_RDWR);
	if (c.respons_.fd_ == -1)
	{
		if (errno == ENOENT)
			return build_error(c, 404);
		if (errno == EACCES)
			return build_error(c, 403);
		if (errno == EISDIR)
			return build_error(c, 403);
		else
			return build_error(c, 500);
	}

	struct stat	file_stat;

	if (fstat(c.respons_.fd_, &file_stat) == -1)
		{ close_fd(&c.respons_.fd_); return build_error(c, 404); }

	if (S_ISDIR(file_stat.st_mode))
		{ close_fd(&c.respons_.fd_); return build_error(c, 403); }

	if (S_ISREG(file_stat.st_mode))
	{
		if (unlink(full_path.c_str()) == -1)
			{ close_fd(&c.respons_.fd_); return build_error(c, 500); }
		c.respons_.status_code_	= 204;
		Logs::info("DELETED " + full_path);
		c.respons_.body_		= "";

		serialize(c);

		return BUILD_STATE::BUILD_OK;
	}

	close_fd(&c.respons_.fd_);
	return build_error(c, 403); 
}

