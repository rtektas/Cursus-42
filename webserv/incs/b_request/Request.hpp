/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 20:34:00 by alephoen          #+#    #+#             */
/*   Updated: 2026/05/04 20:34:00 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <b_request/ParsingStructs.hpp>
#include <g_utils/Types.hpp>

// Request headers and body should be handled by parser;

class	Request
{
private:
    Request(const Request&);
    Request& operator=(const Request&);

public:
	HEADERS	headers_;
	HEADERS	unknown_headers_;

	STR		&HOST_;
	SIZET	LENT_;
	STR		&TYPE_;
	bool	TRNS_;
	bool	CONN_;
	STR		&COOK_;
	bool	EXPT_;

	STR		body_file_path_;	// this will not be used because of unlink()
	int		body_file_fd_;
	SIZET	body_size_;

	SIZET	header_cnt_;

	STR		req_line_;
	STR		body_;
	STR		path_;
	STR		path_tmp_;
	STR		method_;
	STR		target_;
	STR		query_;
	STR		version_;
	bool	parsed_;
	bool	is_req_ready_;
	bool	is_error_;

	~Request() {}

	void	reset();

	Request()
    :   headers_(6, Header()),
        HOST_(headers_[HEADER_IDX::HOST].val),
        LENT_(0),
        TYPE_(headers_[HEADER_IDX::TYPE].val),
        TRNS_(false),
        CONN_(false),
        COOK_(headers_[HEADER_IDX::COOK].val),
		EXPT_(false),
        body_file_fd_(-1),
        body_size_(0),
		header_cnt_(0),
        parsed_(false),
        is_req_ready_(false),
        is_error_(false)
        {}

};

#endif

