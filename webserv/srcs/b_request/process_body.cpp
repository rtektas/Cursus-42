/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_body.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 12:01:18 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/29 13:08:06 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <b_request/ParsingStructs.hpp>
#include <b_request/HTTParser.hpp>
#include <d_client/Client.hpp>
#include <g_utils/Logs.hpp>

static int		open_random_tmp_file(char tmp_path[]);

PARSING_STATE::STATE HTTParser::process_body(Client &c, const char *buffer, SIZET byts_r)
{
	PARSING_STATE::STATE	parsing_state;

	if (!c.request_.TRNS_ && !c.request_.LENT_)
		return PARSING_STATE::PARSING_DONE;		// no body!

	if (c.request_.TRNS_)
		parsing_state = 
			parse_body_by_chunk
				(c, (const unsigned char *) buffer, 
				 byts_r, this->chunk_body_state_
				);
	else
		parsing_state = 
			parse_body_by_len(c, (const unsigned char *) buffer, byts_r);

	return (parsing_state);
}

#include <stdint.h>
static uint8_t hex_to_int(char c)
{
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	return 0;
}

PARSING_STATE::STATE 
						HTTParser::parse_body_by_chunk
						(Client &c,
						 const unsigned char *buf, 
						 SSIZET byts_r, 
						 CHUNK_BODY_STATE::STATE &state)
{
	Request &r = c.request_;

	#define ch buf[i]
	SSIZET	i = -1;
	while (++i < byts_r)
	{
		switch (state)
		{
			case CHUNK_BODY_STATE::CHUNK_SIZE_LINE:
			{
				if (std::isxdigit(ch))
				{
					if (current_chunk_size_ == (SIZET)-1)
						current_chunk_size_ = 0;
					current_chunk_size_ = current_chunk_size_ * 16 + hex_to_int(ch); 
					if (current_chunk_size_ > max_body_size_ )
					{
						c.respons_.status_code_ = 413;
						return PARSING_STATE::PARSING_FATAL_ERR;
					}
				}
				else if (ch == '\r')
				{
					if (current_chunk_size_ == (SIZET)-1)
						return PARSING_STATE::PARSING_FATAL_ERR;
					state = CHUNK_BODY_STATE::CHUNK_SIZE_LINE_CR;
				}
				else
					return PARSING_STATE::PARSING_FATAL_ERR;
				break ;
			}
			case CHUNK_BODY_STATE::CHUNK_SIZE_LINE_CR:
			{
				if (ch != '\n')
					return PARSING_STATE::PARSING_FATAL_ERR;	// 400
				if (current_chunk_size_ == 0)
					state = CHUNK_BODY_STATE::CHUNK_TRAILER;
				else
					state = CHUNK_BODY_STATE::CHUNK_DATA;
				break ;
			}
			case CHUNK_BODY_STATE::CHUNK_DATA:
			{
				SIZET remaining_in_buf = byts_r - i;
				SIZET remaining_in_chunk = current_chunk_size_;
				SIZET span = (remaining_in_buf < remaining_in_chunk) ? 
									remaining_in_buf : remaining_in_chunk;

				if (r.body_size_ < BODY_MEMORY_THRESHOLD)
				{
					SIZET room_in_memory = BODY_MEMORY_THRESHOLD - r.body_size_;
					if (span > room_in_memory)
						span = room_in_memory;	// don't let one span straddle the memory/file boundary
				}

				if (r.method_ == "POST")
				{
					if (r.body_size_ < BODY_MEMORY_THRESHOLD)
					{
						r.body_.append(reinterpret_cast<const char *>(buf + i), span);
					}
					else
					{
						if (r.body_file_fd_ == -1)
						{
							char	tmp_path[] = "/tmp/webserv_req_body_XXXXXX";
							r.body_file_fd_ = open_random_tmp_file(tmp_path);
							if (r.body_file_fd_ == -1)
							{
								c.respons_.status_code_ = 413;
								return PARSING_STATE::PARSING_FATAL_ERR;	// 413
							}

							SSIZET byts_w = write(r.body_file_fd_, r.body_.c_str(), r.body_.size());
							if (byts_w < static_cast<SSIZET>(r.body_.size()))
							{
								c.respons_.status_code_ = 413;
								return PARSING_STATE::PARSING_FATAL_ERR;	// 413
							}
							r.body_.clear();
						}

						SSIZET byts_w = write(r.body_file_fd_, buf + i, span);
						if (byts_w < static_cast<SSIZET>(span))
						{
							c.respons_.status_code_ = 413;
							return PARSING_STATE::PARSING_FATAL_ERR;	// 413
						}
					}
				}

				r.body_size_ += span;
				current_chunk_size_ -= span;
				i += span - 1;	// -1 since the outer while loop's ++i adds 1 back

				if (r.body_size_ > max_body_size_ )
				{
					c.respons_.status_code_ = 413;
					return PARSING_STATE::PARSING_FATAL_ERR;	// 413
				}
				if (current_chunk_size_ == 0)
					state = CHUNK_BODY_STATE::CHUNK_AFTER_DATA;
				break ;
			}
			case CHUNK_BODY_STATE::CHUNK_AFTER_DATA:
			{
				if (ch == '\r')
					state = CHUNK_BODY_STATE::CHUNK_DATA_CR;
				else
					return PARSING_STATE::PARSING_FATAL_ERR;	// 400
				break ;
			}
			case CHUNK_BODY_STATE::CHUNK_DATA_CR:
			{
				if (ch != '\n')
					return PARSING_STATE::PARSING_FATAL_ERR;	// 400
				current_chunk_size_ = (SIZET)-1;	// reset for next chunk
				state = CHUNK_BODY_STATE::CHUNK_SIZE_LINE;
				break ;
			}
			case CHUNK_BODY_STATE::CHUNK_TRAILER:
			{
				++trailer_size_;
				if (trailer_size_ > MAX_TRAILER_SIZE)
					return PARSING_STATE::PARSING_FATAL_ERR;	// 400
				if (ch == '\r')
					state = CHUNK_BODY_STATE::CHUNK_TRAILER_CR;
				break ;
			}
			case CHUNK_BODY_STATE::CHUNK_TRAILER_CR:
			{
				++trailer_size_;
				if (trailer_size_ > MAX_TRAILER_SIZE)
					return PARSING_STATE::PARSING_FATAL_ERR;	// 400
				if (ch == '\n')
				{
					trailer_size_ = 0;
					return PARSING_STATE::PARSING_DONE;
				}
				else
					return PARSING_STATE::PARSING_FATAL_ERR;	// 400
				break ;
			}
		}
	}
	#undef ch
	return PARSING_STATE::PARSING_WAITING;
}

PARSING_STATE::STATE HTTParser::
								parse_body_by_len
								(Client &c, 
								 const unsigned char *buffer, 
								 SSIZET byts_r)
{
	if (!byts_r)
        return PARSING_STATE::PARSING_WAITING;

	if (c.request_.LENT_ > max_body_size_)
	{
		c.respons_.status_code_ = 413;
		return PARSING_STATE::PARSING_FATAL_ERR;	// 413
	}

	if (c.request_.method_ != "POST")
	{
		c.request_.body_size_ += byts_r;
		if (c.request_.body_size_ > max_body_size_)
		{
			c.respons_.status_code_ = 413;
			return PARSING_STATE::PARSING_FATAL_ERR;	// 413
		}
		if (c.request_.body_size_ >= c.request_.LENT_)
			return PARSING_STATE::PARSING_DONE;
        return PARSING_STATE::PARSING_WAITING;
	}

	if (c.request_.LENT_ > BODY_MEMORY_THRESHOLD)
		return  write_req_body_by_len_to_tmp_file(c, buffer, byts_r);
	
	// left-overs from r_buf_ should been already flushed into body_ before
	// arriving at this point!
	c.request_.body_.append(reinterpret_cast<const char *>(buffer), byts_r);
	c.request_.body_size_ += byts_r;
	if (c.request_.body_size_ > max_body_size_)
	{
		c.respons_.status_code_ = 413;
		return PARSING_STATE::PARSING_FATAL_ERR;	// 413
	}

	if (c.request_.body_size_ < c.request_.LENT_)
        return PARSING_STATE::PARSING_WAITING;

	return PARSING_STATE::PARSING_DONE;
}

#include <stdlib.h>

static int		open_random_tmp_file(char tmp_path[])
{

	int	fd = mkstemp(tmp_path);
	if (fd == -1)
		return (-1);
	unlink(tmp_path);
	return (fd);
}

PARSING_STATE::STATE HTTParser::
	write_req_body_by_len_to_tmp_file(Client &c, const char unsigned *buffer, SSIZET byts_r)
{
	c.request_.body_size_ += byts_r;
	if (c.request_.body_size_ > max_body_size_)
	{
		c.respons_.status_code_ = 413;
		return PARSING_STATE::PARSING_FATAL_ERR;	// 413
	}

	if (c.request_.body_file_fd_ == -1)
	{
		char	tmp_path[] = "/tmp/webserv_req_body_XXXXXX";
		c.request_.body_file_fd_ = open_random_tmp_file(tmp_path);
		if (c.request_.body_file_fd_ == -1)
		{
			c.respons_.status_code_ = 413;
			return PARSING_STATE::PARSING_FATAL_ERR;	// 413
		}
	}

	SSIZET byts_w = write(c.request_.body_file_fd_, buffer, byts_r);
	if (byts_w < byts_r)
	{
		c.respons_.status_code_ = 413;
		return PARSING_STATE::PARSING_FATAL_ERR;	// 413
	}

	if (c.request_.body_size_ < c.request_.LENT_)
		return PARSING_STATE::PARSING_WAITING;

	return PARSING_STATE::PARSING_DONE;
}

