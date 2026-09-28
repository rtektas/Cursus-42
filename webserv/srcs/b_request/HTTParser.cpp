/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 08:14:43 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/23 08:14:43 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <a_config_parser/RouteConf.hpp>
#include <b_request/HTTParser.hpp>
#include <d_client/Client.hpp>
#include <b_request/ParsingStructs.hpp>
#include <g_utils/Types.hpp>

#include <cstring>
#include <cctype>
#include <climits>
#include <cstddef>

#include <cstdlib>

#include <unistd.h>
#include <sys/types.h>

#include <cstdio>

bool	is_buffer_nulled(char *buffer, SIZET byts_r)
{
	SIZET i = (SIZET)-1;
	while (++i < byts_r)
		if (buffer[i] == '\0')
			return (true);
	return (false);
}

PARSING_STATE::STATE
	HTTParser::parse_request(Client &c, char *buffer, SIZET byts_r)
{
	PARSING_STATE::STATE	parsing_state;

	if (c.r_buf_size_ >= MAX_READ_BUFFER_SIZE)
		return PARSING_STATE::PARSING_FATAL_ERR;

	if (!body_started_ && is_buffer_nulled(buffer, byts_r))
		return PARSING_STATE::PARSING_FATAL_ERR;

    if (!is_req_line_found_)
    {
		parsing_state = process_req_line(c, buffer, byts_r);
		if (r_buf_pos_ > byts_r)
			return PARSING_STATE::PARSING_FATAL_ERR;

		if (r_buf_pos_    == byts_r - 1 && 
			parsing_state == PARSING_STATE::PARSING_WAITING)
			return parsing_state;


		if (parsing_state == PARSING_STATE::PARSING_FATAL_ERR)
			return parsing_state;

		if (is_req_line_found_)
			state_ = STATE_MACHINE::NO_CR;
		++r_buf_pos_;
		buffer += r_buf_pos_;
		byts_r -= r_buf_pos_;
    }
	
	bool headers_were_done = end_of_headers_;

	SIZET	body_offset = 0;

	if (!end_of_headers_)
	{
		SIZET	cnt = 0;

		while (true)
		{
			if (*(buffer + cnt) == '\0')
				return PARSING_STATE::PARSING_WAITING;

			parsing_state = process_headers(c, buffer + cnt, byts_r - cnt); 
			if (parsing_state == PARSING_STATE::PARSING_FATAL_ERR)
				return parsing_state;

			if (end_of_headers_)
			{
				body_offset = cnt + r_buf_pos_ + 1;
				if (body_offset > byts_r)
					body_offset = byts_r;
				break ;
			}
			if (cnt + r_buf_pos_ == byts_r - 1 && 
				parsing_state == PARSING_STATE::PARSING_WAITING)
				return parsing_state;
			else
			if (r_buf_pos_ != (SIZET)-1 && r_buf_pos_ > byts_r)
				return PARSING_STATE::PARSING_FATAL_ERR;

			cnt += r_buf_pos_ + 1;
			state_ = STATE_MACHINE::NO_CR;
		}
	}

	if (!done_routing_)
	{
			if (c.request_.headers_[HEADER_IDX::HOST].key.empty())
			{
				c.respons_.is_ready_ = true;
				c.request_.is_error_ = true;
				c.respons_.status_code_ = 400;
				return PARSING_STATE::PARSING_FATAL_ERR;
			}

		parsing_state = route_request(c);
		if (parsing_state == PARSING_STATE::PARSING_FATAL_ERR)
			return parsing_state;
	}

	if (!body_started_)
	{
		body_started_ = true;

		if (headers_were_done)
			parsing_state = process_body(c, buffer, byts_r);
		else
			parsing_state = 
				process_body(c, buffer + body_offset, byts_r - body_offset);

		c.r_buf_.clear();
			
		return parsing_state;
	}

	parsing_state = process_body(c, buffer, byts_r);
	if (parsing_state == PARSING_STATE::PARSING_WAITING ||
		parsing_state == PARSING_STATE::PARSING_FATAL_ERR)
		return parsing_state;

	return PARSING_STATE::PARSING_DONE;
}

int		open_random_tmp_file(char tmp_path[])
{
	int	fd = mkstemp(tmp_path);
	if (fd == -1)
		return (-1);
	unlink(tmp_path);
	return (fd);
}

PARSING_STATE::STATE HTTParser::route_request(Client &c)
{
	new (&c.route_data_) 
		RouteData(c.route_conf_.getRouteData
					(c.server_port_, c.request_.HOST_, c.request_.path_));

	if (c.route_data_.rout_locatn_ == NULL)
		return PARSING_STATE::PARSING_FATAL_ERR;	// 500

	if (c.route_data_.meth_locatn_ == NULL)
		return PARSING_STATE::PARSING_FATAL_ERR;	// 500

	if (c.route_data_.client_max_body_size_locatn_ == 0)
		this->max_body_size_ = MAX_BODY_SIZE;
	else
		this->max_body_size_ = c.route_data_.client_max_body_size_locatn_;

	done_routing_ = true;

	return PARSING_STATE::PARSING_DONE;
}

void		HTTParser::reset()
{
	// request line / header flags
	is_req_line_found_		= false;
	end_of_headers_			= false;
	done_routing_			= false;
	body_started_			= false;

	// state machine
	state_					= STATE_MACHINE::NO_CR;

	// body parsing
	max_body_size_			= 0;
	r_buf_pos_				= (SIZET)-1;
	body_size_				= 0;
	body_.clear(); 

	// chunked body state machine
	chunk_body_state_		= CHUNK_BODY_STATE::CHUNK_SIZE_LINE;
	current_chunk_size_		= (SIZET)-1;
	trailer_size_			= 0;
	chunk_size_line_.clear();
	chunk_body_data_.clear();
}

int	close_fd(int *fd);

void		Request::reset()
{
	// clear header slots references still point to the same slots, now empty!
	headers_.assign(6, Header());
	unknown_headers_.clear();

	// reset primitives
	LENT_	= 0;
	TRNS_	= false;
	CONN_	= false;
	EXPT_	= false;

	// reset body
	if (body_file_fd_ != -1)
		close_fd(&body_file_fd_);
	body_file_fd_	= -1;
	body_size_		= 0;
	header_cnt_		= 0;
	body_.clear();

	// reset request line fields
	path_.clear();
	path_tmp_.clear();
	method_.clear();
	target_.clear();
	query_.clear();
	version_.clear();
	req_line_.clear();

	// reset flags
	parsed_			= false;
	is_req_ready_	= false;
	is_error_		= false;

}

// #include <fcntl.h>
// char g_log_buf[LOG_BUF_SIZE];
// volatile sig_atomic_t g_log_pos = 0;
// 
// void dump_log_handler(int /*sig*/)
// {
    // int fd = open("/tmp/dumped_log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    // if (fd != -1)
    // {
        // write(fd, g_log_buf, g_log_pos);
        // close(fd);
    // }
// }

