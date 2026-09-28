/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 19:56:19 by alephoen          #+#    #+#             */
/*   Updated: 2026/05/04 19:56:19 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <a_config_parser/RouteConf.hpp>

#include <b_request/HTTParser.hpp>
#include <b_request/Request.hpp>
#include <b_request/ParsingStructs.hpp>

#include <c_response/ResponseBuilder.hpp>
#include <c_response/Response.hpp>
#include <c_response/BuilderStructs.hpp>

#include <f_cgi/CGIHandler.hpp>

#include <g_utils/Types.hpp>

#include <ctime>
#include <sys/types.h>

class HTTParser;
class CGIHandler;
class ResponseBuilder;
class ConnectionManager;

class	Client
{
private:
	int				fd_;
	STR				ip_;
	int				port_;
	int				server_port_;
	bool			is_active_;

	// buffers
	STR				w_buf_;
	SIZET			w_pos_;

	STR				r_buf_;
	SIZET			r_buf_size_;
	SIZET			headers_sum_size_;		// not implemented!

	time_t			last_activity_;

	// higher-level objects;
	Request			request_;
	Response		respons_;

	RouteData		route_data_;

	HTTParser		parser_;
	CGIHandler		cgi_;
	ResponseBuilder	builder_;

	const RouteConf&	route_conf_;

	Client(const Client&);
	Client&	operator=(const Client&);

	friend class HTTParser;
	friend class CGIHandler;
	friend class ResponseBuilder;
	friend class ConnectionManager;
	friend class Server;

public:
	~Client() {}

	Client(const RouteConf&	route_conf) 
		:	fd_(-1), ip_(""), port_(0), server_port_(0), is_active_(false),
			w_buf_(""), w_pos_(0), r_buf_(""), r_buf_size_(0), headers_sum_size_(0),
			last_activity_(time(NULL)), route_conf_(route_conf)
	{}

	void	activate(int fd, const STR &ip, int port, int server_port);
	void	deactivate();
	void	keepalive_reset();

	READ_STATE::STATE	on_read();
	WRIT_STATE::STATE	on_writ();

	short		get_poll_events()	const;

	int			get_cgi_in_fd()		const;
	int			get_cgi_ut_fd()		const;

	bool		is_cgi_running()	const;

	void		build_error_response(int code);
};

#endif

