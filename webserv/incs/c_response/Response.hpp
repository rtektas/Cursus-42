/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 20:48:12 by alephoen          #+#    #+#             */
/*   Updated: 2026/05/04 20:48:12 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <c_response/BuilderStructs.hpp>
#include <g_utils/Types.hpp>

#include <sys/stat.h>

// Response headers and body should be handled by ResponseBuilder

class	Response
{
private:
	Response(const Response&);
	Response&	operator=(const Response&);

	friend class Client;
	friend class Server;
public:
	int		fd_;
	struct stat	file_stat_;

	HEADERS	headers_;
	STR		full_path_;
	STR		body_;
	bool	is_ready_;
	bool	is_error_;
	bool	is_chunked_;
	int		status_code_;
	STR		content_type_;

	STR		TYPE_BOUNDARY_VAL_;

	Response()
		: fd_(-1),
		  file_stat_(),
		  headers_(),
		  full_path_(),
		  body_(),
		  is_ready_(false),
		  is_error_(false),
		  is_chunked_(false),
		  status_code_(0),
		  content_type_(),
		  TYPE_BOUNDARY_VAL_()
	{}

	void	reset();

};

#endif

