/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:06:26 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/28 21:31:26 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <f_cgi/CGIHandler.hpp>
#include <c_response/ResponseBuilder.hpp>
#include <d_client/Client.hpp>
#include <g_utils/Types.hpp>
#include <g_utils/utils.hpp>
#include <cstring>
#include <stdlib.h>
#include <fcntl.h>
#include <g_utils/Logs.hpp>

#include <signal.h>
#include <sys/wait.h>
#include <limits.h>
#include <stdlib.h>

int	close_fd(int *fd);

template<typename T>
std::string to_string98(const T& value);

BUILD_STATE::STATE	CGIHandler::cgi_exec(Client &c, STR script_path)
{
	int	fd_ut[2] = {-1, -1};
	if (pipe(fd_ut) == -1)
		return (Logs::error("CGI PIPE FAILED"), BUILD_STATE::BUILD_FATAL_ERR);

	int	fd_in[2] = {-1, -1};
	if (!c.request_.body_.empty())
	{
		if (pipe(fd_in) == -1)
		{
			Logs::error("CGI PIPE STDIN FAILED");
			close_fd(&(fd_ut[0]));
			close_fd(&(fd_ut[1]));
			return BUILD_STATE::BUILD_FATAL_ERR;
		}
	}

	pid_t	pid = fork();
	if (pid == -1)
	{
		Logs::error("CGI FORK FAILED");
		close_fd(&(fd_ut[0]));
		close_fd(&(fd_ut[1]));
		close_fd(&(fd_in[0]));
		close_fd(&(fd_in[1]));
		return BUILD_STATE::BUILD_FATAL_ERR;
	}
	if (pid == 0)
	{
		if (c.request_.body_file_fd_ != -1)
		{
			lseek(c.request_.body_file_fd_, 0, SEEK_SET);
			if (dup2(c.request_.body_file_fd_, 0) == -1)
				exit(1);
			close_fd(&c.request_.body_file_fd_);
		}
		else
		if (!c.request_.body_.empty())
		{
			if (dup2(fd_in[0], 0) == -1)
				exit(1);
			close_fd(&(fd_in[0]));
			close_fd(&(fd_in[1]));
		}
		else
		{
			int devnull_r = open("/dev/null", O_RDONLY);
			if (devnull_r == -1)
				exit(1);
			if (dup2(devnull_r, 0) == -1)
				exit(1);
			close_fd(&devnull_r);
		}

		if (dup2(fd_ut[1], 1) == -1)
				exit(1);
		close_fd(&(fd_ut[0]));
		close_fd(&(fd_ut[1]));

		int devnull_w = open("/dev/null", O_WRONLY);
		if (devnull_w == -1)
			exit(1);

		if (dup2(devnull_w, 2) == -1)
			exit(1);
		close_fd(&devnull_w);


		VEC_STR		envs_strs;
		VEC_CSTR	envs_ptrs;

		this->cgi_env(c, envs_strs, envs_ptrs);

		char abs_interp[PATH_MAX];
		char abs_script[PATH_MAX];

		realpath(interpreter_.c_str(), abs_interp);
		realpath(script_path.c_str(), abs_script);

		STR script_dir = script_path.substr(0, script_path.find_last_of('/'));
		chdir(script_dir.c_str());

		char *av[3]; 
		av[0] = abs_interp;
		av[1] = abs_script;
		av[2] = NULL;

		execve(av[0], av, &(envs_ptrs[0]));
		exit(1);
	}
	
	Logs::info("CGI FORK pid=" + to_string98(pid) + " script=" + 
						script_path + " interpreter=" + interpreter_);

	close_fd(&(fd_ut[1]));
	fcntl(fd_ut[0], F_SETFL, O_NONBLOCK);
	c.cgi_.cgi_ut_fd_		= fd_ut[0];
	c.cgi_.cgi_pid_		= pid;
	c.cgi_.cgi_beg_t_	= time(NULL);

	if (c.request_.body_file_fd_ != -1)
		c.cgi_.cgi_in_fd_ = -1;
	else
	if (!c.request_.body_.empty())
	{
		close_fd(&(fd_in[0]));
		fcntl(fd_in[1], F_SETFL, O_NONBLOCK);
		c.cgi_.cgi_in_fd_		= fd_in[1];
		c.cgi_.cgi_body_pos_	= 0;
	}
	else
	{
		close_fd(&(fd_in[1]));
		c.cgi_.cgi_in_fd_		= -1;
	}

	return BUILD_STATE::BUILD_CGI_RUNNING;
}

