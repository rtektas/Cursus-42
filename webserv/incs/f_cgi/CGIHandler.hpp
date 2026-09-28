/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 18:29:05 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/02 18:29:05 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <c_response/BuilderStructs.hpp>
#include <g_utils/Types.hpp>

#include <sys/types.h>

class Client;
class ResponseBuilder;

class	CGIHandler
{
private:
	pid_t	cgi_pid_;

	STR		interpreter_;

	int		cgi_in_fd_;
	int		cgi_ut_fd_;

	time_t	cgi_beg_t_;

	SIZET	cgi_body_pos_;
	STR		cgi_buf_;

	bool	cgi_headers_sent_;

	BUILD_STATE::STATE	cgi_exec(Client &c, STR script_path);

	void	cgi_env(Client  &c, VEC_STR &envs_strs, VEC_CSTR &envs_ptrs);

	void				kill_cgi();
	void				reset();

	void				build_env(Client	&c, 
								  const STR &script_path,
								  const STR &interpreter,
								  VEC_STR   &envs_strs,
								  VEC_CSTR  &envs_ptrs);

	CGIHandler(const CGIHandler&);
	CGIHandler&		operator=(const CGIHandler&);

	BUILD_STATE::STATE	parse_interpreter(Client &c);

	friend class Client;
	friend class ResponseBuilder;
	friend class Server;

public:
	~CGIHandler() {}

	CGIHandler() 
		:	cgi_pid_(-1),  cgi_in_fd_(-1), cgi_ut_fd_(-1),
			cgi_beg_t_(0), cgi_body_pos_(0), cgi_headers_sent_(0)
	{}

	BUILD_STATE::STATE	exec(Client &c);
	BUILD_STATE::STATE	on_stdin_ready(Client &c);
	BUILD_STATE::STATE	on_stdut_ready(Client &c);

	BUILD_STATE::STATE	check_timeout(Client &c);

	bool				is_cgi_running()	const;
};

#endif

