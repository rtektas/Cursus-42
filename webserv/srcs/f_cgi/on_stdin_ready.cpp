/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_stdin_ready.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:09:12 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/23 17:09:12 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <c_response/ResponseBuilder.hpp>
#include <d_client/Client.hpp>
#include <g_utils/Types.hpp>
#include <g_utils/utils.hpp>
#include <g_utils/Logs.hpp> 

int	close_fd(int *fd);

template<typename T>
std::string to_string98(const T& value);
BUILD_STATE::STATE CGIHandler::on_stdin_ready(Client &c)
{
	STR &body = c.request_.body_;
	SIZET	size = body.size() - cgi_body_pos_;

	SSIZET	byts_w = write(cgi_in_fd_, body.c_str() + cgi_body_pos_, size);
	if (byts_w == -1)
	{
		Logs::error("CGI WRITE ERR in_fd=" + to_string98(cgi_in_fd_));
		kill_cgi();
		return c.builder_.build_error(c, 500);
	}

	cgi_body_pos_ += byts_w;

	if (cgi_body_pos_ >= body.size())
		close_fd(&cgi_in_fd_);

	return BUILD_STATE::BUILD_CGI_RUNNING;
}

