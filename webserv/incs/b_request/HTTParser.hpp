/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 08:29:04 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/23 08:29:04 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	HTPP_PARSER_HPP
#define HTPP_PARSER_HPP

#include <unistd.h>
#include <sys/types.h>
#include <b_request/ParsingStructs.hpp>
#include <b_request/Request.hpp>
#include <g_utils/Types.hpp>

class	Client;

class	HTTParser
{
private:
	bool	is_req_line_found_;
	bool	end_of_headers_;
	bool	done_routing_;
	bool	body_started_;

	SIZET	trailer_size_;

	SIZET	max_body_size_;

	CHUNK_BODY_STATE::STATE	chunk_body_state_;
	STR		chunk_size_line_;
	STR		chunk_body_data_;
	STR		body_;
	SIZET	current_chunk_size_;
	SIZET	body_size_;
	
	SIZET	r_buf_pos_;

	STATE_MACHINE::STATE	state_;

	// ======================================================================
	//						parse_request()
	// ======================================================================
	PARSING_STATE::STATE
		process_req_line(Client &c, char *buffer, SIZET byts_r);

	PARSING_STATE::STATE
		process_headers(Client &c, char *buffer, SIZET byts_r);

	PARSING_STATE::STATE route_request(Client &c);

	PARSING_STATE::STATE process_body
							(Client &c, const char *buffer, SIZET byts_r);
	// ======================================================================

	// ======================================================================
	//						process_req_line()
	// ======================================================================
	HEADER_STATE::STATE		
		check_req_header_state(Client &c, char *buffer, SIZET byts_r);

	PARSING_STATE::STATE	parse_req_line(Client &c);

	PARSING_STATE::STATE	decode_path(Client &c);

	PARSING_STATE::STATE	normalize_path(Client &c);

	PARSING_STATE::STATE	
		check_parsing_state(HEADER_STATE::STATE buf_state);
	// ======================================================================

	STATE_MACHINE::STATE	
		check_req_buf_state
			(char *buffer, SIZET byts_r, 
			 STATE_MACHINE::STATE &state, SIZET *byts_cnt);

	int		parse_body(const STR &buf, Request &req);
	bool	detect_chunked(const STR &headers);
	STR		extract_path(const STR &headers);
	SIZET	parse_content_len(const STR &headers);


	static int	parse_header(const STR &str, t_Header &header);

	PARSING_STATE::STATE 
				parse_body_by_chunk
				(Client &c,
				 const unsigned char *buf, 
				 SSIZET byts_r, 
				 CHUNK_BODY_STATE::STATE &state);

	PARSING_STATE::STATE 
				parse_body_by_len
				(Client &c, 
				 const unsigned char *buffer, 
				 SSIZET byts_r);

	PARSING_STATE::STATE 
		write_req_body_by_len_to_tmp_file
		(Client &c, const char unsigned *buffer, SSIZET byts_r);

	t_Header
	parse_header(STR header_str, PARSING_STATE::STATE *state);

	HTTParser(const HTTParser &s);
	HTTParser&	operator=(const HTTParser &s);

	friend class Client;

public:
	~HTTParser() {}

	HTTParser()
		: is_req_line_found_(false),
		  end_of_headers_(false),
		  done_routing_(false),
		  body_started_(false),
		  trailer_size_(0),
		  max_body_size_(0),
		  chunk_body_state_(CHUNK_BODY_STATE::CHUNK_SIZE_LINE),
		  chunk_size_line_(""),
		  chunk_body_data_(""),
		  body_(""),
		  current_chunk_size_((SIZET)-1),
		  body_size_(0),
		  r_buf_pos_((SIZET)-1),
		  state_(STATE_MACHINE::NO_CR)
	{}

	PARSING_STATE::STATE	
		parse_request(Client &c, char *buffer, SIZET byts_r);

	void	reset();
};

// #include <csignal>
// #include <cstdarg>
// #include <cstdio>
// 
// #define LOG_BUF_SIZE (32 * 1024 * 1024)
// extern char g_log_buf[LOG_BUF_SIZE];
// extern volatile sig_atomic_t g_log_pos;
// 
// void dump_log_handler(int sig);
// 
// inline void log_append(const char *fmt, ...)
// {
    // if (g_log_pos >= LOG_BUF_SIZE - 256)
        // return;
    // va_list args;
    // va_start(args, fmt);
    // int written = 
		// vsnprintf(g_log_buf + g_log_pos, LOG_BUF_SIZE - g_log_pos, fmt, args);
    // va_end(args);
    // if (written > 0)
        // g_log_pos += written;
// }

#endif

