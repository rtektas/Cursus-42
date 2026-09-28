/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 14:45:27 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/29 13:46:04 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <d_client/Client.hpp>
#include <g_utils/Types.hpp>
#include <g_utils/Logs.hpp>

#include <sys/types.h>
#include <sys/poll.h>
#include <sys/socket.h>

template<typename T>
std::string to_string98(const T& value);
void	Client::activate(int fd, const STR &ip, int port, int server_port)
{
	fd_				= fd;
	ip_				= ip;
	port_			= port;
	server_port_	= server_port;
	is_active_		= true;
	last_activity_	= time(NULL);
}

int	close_fd(int *fd);

void	Client::deactivate()
{
	close_fd(&fd_);
	fd_				= -1;
	ip_				= "";
	port_			= 0;
	server_port_	= 0;
	is_active_		= false;

	headers_sum_size_	= 0;

	last_activity_ = time(NULL);

	r_buf_.clear();
	r_buf_size_ = 0;

	w_buf_.clear();
	w_pos_		= 0;

	parser_.reset();
	request_.reset();
	respons_.reset();
	cgi_.reset();
}

void	Client::keepalive_reset()
{
	parser_.reset();
	request_.reset();

	respons_.reset();

	cgi_.reset();

	r_buf_.clear();
	r_buf_size_ = 0;
	w_buf_.clear();
	w_pos_ = 0;

	last_activity_ = time(NULL);
}

short	Client::get_poll_events()	const
{
	short	events = 0;

	if (!request_.is_req_ready_)
		events |= POLLIN;

	if (w_pos_ < w_buf_.size())
		events |= POLLOUT;

	if (request_.is_req_ready_ && !respons_.is_ready_ && !cgi_.is_cgi_running())
		events |= POLLOUT;

	return (events);
}

READ_STATE::STATE Client::on_read()
{
	char	buf[MAX_READ_BUFFER_SIZE] = {0};

	ssize_t	byts_r = recv(fd_, buf, sizeof(buf), 0);
	if (byts_r < 0)
	{
		Logs::warn("READ ERR fd=" + to_string98(fd_));
		return (READ_STATE::READ_ERROR);
	}
	else if (byts_r == 0)
		return (READ_STATE::READ_CLOSE);

	last_activity_ = time(NULL);

	if (r_buf_.empty())
		r_buf_size_ = 0;
	
	PARSING_STATE::STATE	parsing_state;
	parsing_state = parser_.parse_request(*this, buf, byts_r);
	switch (parsing_state) 
	{
		case PARSING_STATE::PARSING_WAITING:
			return READ_STATE::READ_OK;			// not done yet, keep reading
		case PARSING_STATE::PARSING_DONE:
		{
			Logs::info("REQ fd=" + to_string98(fd_) + " " + request_.method_ + 
					" " + request_.path_ + " " + request_.version_);

			request_.is_req_ready_ = true;		// signal response building
			return READ_STATE::READ_DONE;		// done reading!
		}
		case PARSING_STATE::PARSING_FATAL_ERR:
			Logs::warn("PARSE ERR fd=" + to_string98(fd_) + " status=" + 
							to_string98(respons_.status_code_));
			request_.is_error_ = true;
			return READ_STATE::READ_FATAL;		// close connection
		default:
			request_.is_error_ = true;
			return READ_STATE::READ_FATAL;		// should never happen but just in case
	}

	request_.is_error_ = true;
	return (READ_STATE::READ_FATAL);
}

WRIT_STATE::STATE Client::on_writ()
{
	if (request_.is_error_)
	{
		request_.CONN_ = true;		// signal that the client should be closed 
									// and keepalive_reset() will be 
									// called for clean-up;
		if (respons_.status_code_)
			build_error_response(respons_.status_code_);
		else
			build_error_response(400);
	}
	else
	if (!respons_.is_ready_)
	{
		if (cgi_.is_cgi_running())
			return WRIT_STATE::WRIT_CGI;
		BUILD_STATE::STATE building_state;
		building_state = builder_.build_response(*this);
		if (building_state == BUILD_STATE::BUILD_FATAL_ERR)
			return WRIT_STATE::WRIT_FATAL;
		if (building_state == BUILD_STATE::BUILD_CGI_RUNNING)
			return WRIT_STATE::WRIT_CGI;
		respons_.is_ready_ = true;
	}

	// send w_buf_ progressively
	SSIZET	byts_s = send(fd_, w_buf_.c_str() + w_pos_, w_buf_.size() - w_pos_, 0);
	if (byts_s < 0)
	{
		Logs::warn("SEND ERR fd=" + to_string98(fd_));
		return WRIT_STATE::WRIT_FATAL;
	}
	w_pos_ += byts_s;

	if (w_pos_ >= w_buf_.size())
	{
		Logs::info("RESP fd=" + to_string98(fd_) + 
				" status=" + to_string98(respons_.status_code_) + 
				" " + request_.method_ + " " + request_.path_);

		return WRIT_STATE::WRIT_DONE;
	}
	return WRIT_STATE::WRIT_OK;
}

void		Client::build_error_response(int code)
{
	if (code == 408)
		request_.CONN_ = true;
	builder_.build_error(*this, code);
	respons_.is_ready_ = true;
}

int		Client::get_cgi_in_fd()	const { return cgi_.cgi_in_fd_; }

int		Client::get_cgi_ut_fd()	const { return cgi_.cgi_ut_fd_; }

