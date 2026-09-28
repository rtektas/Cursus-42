/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_stdut_ready.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:09:26 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/23 17:09:26 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <c_response/ResponseBuilder.hpp>
#include <d_client/Client.hpp>
#include <g_utils/Types.hpp>
#include <g_utils/utils.hpp>
#include <g_utils/Logs.hpp>
#include <sys/wait.h> // waitpid, WNOHANG

int	close_fd(int *fd);

BUILD_STATE::STATE CGIHandler::on_stdut_ready(Client &c)
{
	char buf[4096];

	SSIZET byts_r = read(cgi_ut_fd_, buf, sizeof(buf));

	if (byts_r == -1)
	{
		Logs::error("CGI READ ERR ut_fd=" + to_string98(cgi_ut_fd_));
		kill_cgi();
		return (c.builder_.build_error(c, 500));
	}

	if (byts_r == 0)
	{
		// EOF - body fully sent, done
		close_fd(&cgi_ut_fd_);
		waitpid(cgi_pid_, NULL, 0);
		cgi_pid_ = -1;
		c.request_.CONN_ = true;
		if (c.w_buf_.empty())
			c.respons_.is_ready_ = true;
		return BUILD_STATE::BUILD_OK;
	}

	if (!cgi_headers_sent_)
	{
		// accumulate until we find \r\n\r\n
		cgi_buf_.append(buf, byts_r);
		STR::size_type headers_end = cgi_buf_.find("\r\n\r\n");

		SIZET	separator_len = 4;
		if (headers_end == STR::npos)
		{
			headers_end		= cgi_buf_.find("\n\n");
			separator_len	= 2;
		}

		if (headers_end == STR::npos)
			return BUILD_STATE::BUILD_CGI_RUNNING;	// need more data;

		// headers complete - parse and build HTTP response headers into w_buf_
		c.builder_.cgi_serialize_headers(c);
		cgi_headers_sent_ = true;

		// append any body bytes that arrived after \r\n\r\n
		STR::size_type body_start = headers_end + separator_len;
		if (body_start < cgi_buf_.size())
			c.w_buf_.append(cgi_buf_, body_start, cgi_buf_.size() - body_start);

		cgi_buf_.clear();	// no longer needed
		
		// signal reasponse is ready so PULLOUT starts firing
		c.respons_.is_ready_ = true;	
	}
	else
	{
		// headers already sent - stream body bytes directly to w_buf_
		c.w_buf_.append(buf, byts_r);
	}

	return BUILD_STATE::BUILD_CGI_RUNNING;
}

