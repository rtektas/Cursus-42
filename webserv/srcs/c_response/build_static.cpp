/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_static.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:58:01 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/23 16:58:01 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <c_response/ResponseBuilder.hpp>
#include <d_client/Client.hpp>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

STR		get_mime_type(const STR &path);
int		close_fd(int *fd);

BUILD_STATE::STATE	ResponseBuilder::build_static(Client &c)
{
	SIZET	file_size = c.respons_.file_stat_.st_size;

	c. respons_.body_.reserve(file_size);

	char	buf[4096];
	SSIZET	byts_r;
	while (true)
	{
		byts_r = read(c.respons_.fd_, buf, sizeof(buf));
		if (byts_r == 0)
			break ;
		if (byts_r < 0)
		{ close_fd(&c.respons_.fd_); return BUILD_STATE::BUILD_FATAL_ERR; }
		c.respons_.body_.append(buf, byts_r);
	}
	close_fd(&c.respons_.fd_);

	if (c.respons_.status_code_ == 0)
		c.respons_.status_code_	= 200;
	c.respons_.content_type_	= get_mime_type(c.respons_.full_path_);

	serialize(c);
	
	return BUILD_STATE::BUILD_OK;
}

// helper function!
STR		get_mime_type(const STR &path)
{
	// Find last dot
	std::size_t	dot = path.rfind('.');
	if (dot == STR::npos)
		return ("text/plain");	// fallback

	STR	ext = path.substr(dot);

	if (ext == ".txt")
		return "text/plain";
	if (ext == ".html" || ext == ".htm")
		return "text/html";
	if (ext == ".css")
		return "text/css";
	if (ext == ".js")
		return "application/javascript";
	if (ext == ".json")
		return "application/json";
	if (ext == ".png")
		return "image/png";
	if (ext == ".jpg" || ext == ".jpeg")
		return "image/jpeg";
	if (ext == ".gif")
		return "image/gif";
	if (ext == ".ico")
		return "image/x-icon";
	if (ext == ".pdf")
		return "application/pdf";

	return "application/octet-stream";		// default for unkown types
}

