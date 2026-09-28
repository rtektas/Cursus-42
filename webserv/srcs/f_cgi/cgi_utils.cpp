/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_utils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:08:45 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/23 17:08:45 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <c_response/ResponseBuilder.hpp>
#include <d_client/Client.hpp>
#include <g_utils/Types.hpp>
#include <g_utils/utils.hpp>
#include <signal.h>   // SIGKILL, SIGTERM, etc.
#include <sys/types.h>
#include <sys/wait.h> // waitpid, WNOHANG
#include <unistd.h>   // kill, fork, execve, etc.
#include <g_utils/Logs.hpp>

int	close_fd(int *fd);

template<typename T>
std::string to_string98(const T& value);

void	CGIHandler::kill_cgi()
{
	if (cgi_pid_ != -1)
	{
		Logs::warn("CGI KILL pid=" + to_string98(cgi_pid_));
		kill(cgi_pid_, SIGKILL);
		waitpid(cgi_pid_, NULL, 0);
		cgi_pid_ = -1;
	}
	close_fd(&cgi_in_fd_);
	close_fd(&cgi_ut_fd_);
}

void	CGIHandler::reset()
{
	kill_cgi();
	interpreter_.clear();
	cgi_buf_.clear();
	cgi_body_pos_ = 0;
	cgi_headers_sent_ = 0;
}


BUILD_STATE::STATE	CGIHandler::check_timeout(Client &c)
{
	(void) c;
	if (cgi_pid_ == -1)
		return BUILD_STATE::BUILD_OK;

	if (time(NULL) - cgi_beg_t_ > CGI_TIMEOUT)
	{
		// std::cout << "cgi timeout!\n";
		Logs::warn("CGI TIMEOUT pid=" + to_string98(cgi_pid_) + " fd=" + to_string98(c.fd_) + " elapsed=" +
  to_string98(time(NULL) - cgi_beg_t_) + "s");
		kill_cgi();
		return c.builder_.build_error(c, 500);
	}

	return BUILD_STATE::BUILD_CGI_RUNNING;
}

bool	CGIHandler::is_cgi_running() const
{
	return (cgi_pid_ != -1);
}

