/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_headers.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 11:26:01 by alephoen          #+#    #+#             */
/*   Updated: 2026/05/29 11:26:01 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <b_request/ParsingStructs.hpp>
#include <b_request/HTTParser.hpp>
#include <d_client/Client.hpp>

#include <g_utils/utils.hpp>

#include <sys/socket.h>
#include <string.h>

namespace process_headers
{
	t_Header	parse_header_key(STR header_str, PARSING_STATE::STATE *state);

	HEADER_NAME::NAME verify_header_name(STR key);

	bool	add_header
				(HEADERS &des, const Header &src, HEADER_NAME::NAME key_name);

	bool	add_unknown_header(HEADERS &des, const Header &src);

	bool	can_add_header(HEADERS &des, SIZET des_id, const Header &src);

	inline bool	is_octet(unsigned char c);
	inline bool	is_tchar(unsigned char c);
	inline bool	is_header_val_char(unsigned char c);

	inline bool	is_octet(unsigned char c)
	{
		if (c <= 32  || c == 127 || c == '\"' ||
			c == ',' || c == ';' || c == '\\')
			return (false);
		return (true);
	}

	inline bool	is_tchar(unsigned char c)
	{
		if (
				std::isalnum(c) ||
				c == '!' || c == '#' || c == '$' || c == '%' ||
				c == '&' || c == '\''|| c == '*' || c == '+' ||
				c == '-' || c == '.' || c == '^' || c == '_' ||
				c == '`' || c == '|' || c == '~'
			)
			return (true);

		return (false);
	}

	inline bool	is_header_val_char(unsigned char c)
	{
		if (c == '\t' || c == ' ')
			return (true);

		if (c < 32 || c == 127)
			return (false);
		return (true);
	}
}

using namespace process_headers;

PARSING_STATE::STATE parse_header_val(t_Header &header, 
									 HEADER_NAME::NAME key_name, 
									 HeaderValReturn &ret);

PARSING_STATE::STATE
HTTParser::process_headers(Client &c, char *buffer, SIZET byts_r)
{
	Request &req = c.request_;
	HeaderValReturn ret(req.LENT_, req.TRNS_, req.CONN_, req.COOK_);

    HEADER_STATE::STATE		buf_state;
	PARSING_STATE::STATE	parse_state;

    buf_state = check_req_header_state(c, buffer, byts_r);

    if (c.r_buf_size_ >= MAX_READ_BUFFER_SIZE)
        return PARSING_STATE::PARSING_FATAL_ERR;

    if (buf_state == HEADER_STATE::HEADER_COMPLET ||
        buf_state == HEADER_STATE::END_OF_HEADERS)
    {
        while (true)
        {
            SIZET header_size = c.r_buf_.find("\r\n");
            if (header_size == STR::npos)
                break;
            if (header_size == 0)				// I can delete the CLRF and break here because my state-machine already knowns what to do next. it means only "\r\n" was found
			{ c.r_buf_.erase(0, 2); break; }	// here state_ already flipped to no_cr after end of headers is found because of a previous flip, I commented!

            STR header_str = c.r_buf_.substr(0, header_size);

            t_Header header = parse_header_key(header_str, &parse_state);
			if (parse_state == PARSING_STATE::PARSING_FATAL_ERR)
				return (parse_state);

			HEADER_NAME::NAME key_name = verify_header_name(header.key);
			parse_state = parse_header_val(header, key_name, ret);
			if (parse_state == PARSING_STATE::PARSING_FATAL_ERR)
				return (parse_state);

			if (!add_header(c.request_.headers_, header, key_name))
				return PARSING_STATE::PARSING_FATAL_ERR;	// This means there is an RCF rule violation and thats a hard error!
			else
			{
				if (++c.request_.header_cnt_ > MAX_HEADERS_NUMBER)
				{
					c.respons_.is_ready_ = true;
					c.request_.is_error_ = true;
					c.respons_.status_code_ = 431;
					return PARSING_STATE::PARSING_FATAL_ERR;
				}

				if (key_name == HEADER_NAME::UNKW)
					if (!add_unknown_header(c.request_.unknown_headers_, header))
						return PARSING_STATE::PARSING_FATAL_ERR;
				c.r_buf_.erase(0, header_size + 2);	// After the header been added to t_Header remove it from the client read buffer!
				if (c.r_buf_ == "\r\n")	// this is not correct
					c.r_buf_.erase();	// this should be CRLF_CRLF	<=== WTF is this? how does it work????
			}
        }
		if (buf_state == HEADER_STATE::END_OF_HEADERS)
		{
			end_of_headers_ = true;

			HEADERS::const_iterator it = c.request_.unknown_headers_.begin();
			HEADERS::const_iterator end = c.request_.unknown_headers_.end();
			while (it != end)
			{
				if (it->key == "expect" && strncasecmp(it->val.c_str(), "100-continue", 12) == 0)
				{ 
					c.request_.EXPT_ = true; 
					SSIZET s = send(c.fd_, "HTTP/1.1 100 Continue\r\n\r\n", 25, 0);
					if (s < 0)
						return PARSING_STATE::PARSING_FATAL_ERR;
					break;
				}
				++it;
			}
		}
    }
    return check_parsing_state(buf_state);
}

t_Header	process_headers::parse_header_key(STR header_str, PARSING_STATE::STATE *state)
{
	t_Header		header = Header();
	SIZET			i;

    SIZET key_size = header_str.find_first_of(" \t:");
	if (key_size == STR::npos || key_size == 0)
		return (*state = PARSING_STATE::PARSING_FATAL_ERR, header);

	if (header_str[key_size] != ':')
		return (*state = PARSING_STATE::PARSING_FATAL_ERR, header);

	// validate header key:
	i = -1;
	while (++i < key_size)
	{
		if (!is_tchar(header_str[i]))	
			return (*state = PARSING_STATE::PARSING_FATAL_ERR, header);
	}

    STR key = header_str.substr(0, key_size);
    key = parsing_utils::tolower_case(key, key_size);
    header.key = key;

	STR   val;
	if (key_size < header_str.size())
		val = header_str.substr(key_size + 1);
	parsing_utils::trim_str(val);

	header.val = val;

    *state = PARSING_STATE::PARSING_OK;
	return (header);
}

HEADER_NAME::NAME process_headers::verify_header_name(STR key)
{
	if (key == "host")
		return HEADER_NAME::HOST;
	if (key == "content-length")
		return HEADER_NAME::LENT;
	if (key == "content-type")
		return HEADER_NAME::TYPE;
	if (key == "transfer-encoding")
		return HEADER_NAME::TRNS;
	if (key == "connection")
		return HEADER_NAME::CONN;
	if (key == "cookie")
		return HEADER_NAME::COOK;

	return HEADER_NAME::UNKW;
}

bool	process_headers::add_header(HEADERS &des, const Header &src, HEADER_NAME::NAME key_name)
{
	switch (key_name)
	{
		case HEADER_NAME::UNKW:	// it will be handled by the caller;
			break ;
		case HEADER_NAME::HOST:
			if (!can_add_header(des, 0, src)) return (false);
			break ;
		case HEADER_NAME::LENT:
			if (!can_add_header(des, 1, src)) return (false);
			break ;
		case HEADER_NAME::TYPE:
			if (!can_add_header(des, 2, src)) return (false);
			break ;
		case HEADER_NAME::TRNS:
			if (!can_add_header(des, 3, src)) return (false);
			break ;
		case HEADER_NAME::CONN:
			if (!can_add_header(des, 4, src)) return (false);
			break ;
		case HEADER_NAME::COOK:
			if (!can_add_header(des, 5, src)) return (false);
			break ;
		default: return (false);
	}
	return (true);
}

bool	process_headers::can_add_header(HEADERS &des, SIZET des_id, const Header &src)
{
	switch (des_id)
	{
		case HEADER_IDX::HOST:
		{
			if (!des.at(HEADER_IDX::HOST).key.empty())
				return (false);
			break ;
		}
		case HEADER_IDX::LENT:
		{
			if ((!des.at(HEADER_IDX::LENT).key.empty() && 
				 des.at(HEADER_IDX::LENT).val != src.val))
				return (false);
			if (!des.at(HEADER_IDX::TRNS).key.empty())
				return (false);
			break ;
		}
		case HEADER_IDX::TRNS:
		{
			if (!des.at(HEADER_IDX::LENT).key.empty())
				return (false);
			break ;
		}
		case HEADER_IDX::COOK:
		{
			if (!des.at(HEADER_IDX::COOK).key.empty() && !src.val.empty())
			{
				des.at(des_id).val += "; ";
				des.at(des_id).val += src.val;
				return (true);
			}
			break ;
		}
		default:
			break;
	}

	if (src.val.empty())
		return (true);

	des.at(des_id).key = src.key;
	des.at(des_id).val = src.val;

	return (true);
}

bool	process_headers::add_unknown_header(HEADERS &des, const Header &src)
{
	if (src.key.empty())
		return (false);
	des.push_back(src);

	return (true);
}

PARSING_STATE::STATE
HTTParser::check_parsing_state(HEADER_STATE::STATE buf_state)
{
    switch (buf_state)
    {
        case HEADER_STATE::NEED_MORE_DATA:
            return PARSING_STATE::PARSING_WAITING;
        case HEADER_STATE::HEADER_COMPLET:
            return PARSING_STATE::PARSING_WAITING;
        case HEADER_STATE::END_OF_HEADERS:
            return PARSING_STATE::PARSING_DONE;
        case HEADER_STATE::HEAD_FATAL_ERR:
            return PARSING_STATE::PARSING_FATAL_ERR;
        default:
            return PARSING_STATE::PARSING_FATAL_ERR;
    }
    return PARSING_STATE::PARSING_FATAL_ERR;
}

