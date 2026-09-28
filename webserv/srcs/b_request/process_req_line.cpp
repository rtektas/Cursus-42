/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_req_line.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 11:13:10 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/29 13:40:16 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <b_request/ParsingStructs.hpp>
#include <b_request/HTTParser.hpp>
#include <d_client/Client.hpp>
#include <g_utils/Logs.hpp>

#include <algorithm>

PARSING_STATE::STATE
HTTParser::process_req_line(Client &c, char *buffer, SIZET byts_r)
{
	PARSING_STATE::STATE	ret_state;
    HEADER_STATE::STATE		buf_state;

    buf_state = check_req_header_state(c, buffer, byts_r);
	if (buf_state == HEADER_STATE::HEAD_FATAL_ERR)
		{ int dum = 0; (void) dum; }

    if (c.r_buf_size_ >= MAX_READ_BUFFER_SIZE)
	{
		c.request_.is_error_ = true;
        return PARSING_STATE::PARSING_FATAL_ERR;
	}

    if (buf_state == HEADER_STATE::HEADER_COMPLET ||
        buf_state == HEADER_STATE::END_OF_HEADERS)
    {
        is_req_line_found_ = true;

        if (parse_req_line(c) == PARSING_STATE::PARSING_FATAL_ERR) // I am getting in here after header request
		{
			c.request_.is_error_ = true;
            return PARSING_STATE::PARSING_FATAL_ERR;
		}

		// remove the request line from the buffer:
		SIZET	req_line_size = c.r_buf_.find("\r\n");
		c.r_buf_.erase(0, req_line_size + 2);

        if (decode_path(c) == PARSING_STATE::PARSING_FATAL_ERR)
		{
			c.request_.is_error_ = true;
            return PARSING_STATE::PARSING_FATAL_ERR;
		}

        if (normalize_path(c) == PARSING_STATE::PARSING_FATAL_ERR)
		{
			c.request_.is_error_ = true;
            return PARSING_STATE::PARSING_FATAL_ERR;
		}
    }

	ret_state = check_parsing_state(buf_state);
    if (ret_state == PARSING_STATE::PARSING_FATAL_ERR)
		c.request_.is_error_ = true;

    return (ret_state);
}

// ------------------------------------------------------------
// Helpers
// ------------------------------------------------------------
bool is_key_correct(const STR *tab, SIZET tab_size, const STR &str)
{
    return std::find(tab, tab + tab_size, str) != tab + tab_size;
}

template<typename T>
std::string to_string98(const T& value);

PARSING_STATE::STATE	HTTParser::parse_req_line(Client &c)
{
    SIZET req_line_size = c.r_buf_.find("\r\n");
    if (req_line_size == STR::npos || req_line_size == 0)
        return PARSING_STATE::PARSING_FATAL_ERR;

    STR req_line_str = c.r_buf_.substr(0, req_line_size);

    SIZET pos = req_line_str.find(' ');
    if (pos == STR::npos || pos == 0)
        return PARSING_STATE::PARSING_FATAL_ERR;

    c.request_.method_ = req_line_str.substr(0, pos);
    const STR METHODS[] = {"GET", "POST", "DELETE"};
    if (!is_key_correct(METHODS, 3, c.request_.method_))
	{
		c.respons_.status_code_ = 405;
		Logs::warn("BAD METHOD: " + c.request_.method_ + " from fd=" + to_string98(c.fd_));
        return PARSING_STATE::PARSING_FATAL_ERR;
	}

    req_line_str.erase(0, pos + 1);

    if (req_line_str.empty() || req_line_str[0] != '/')
        return PARSING_STATE::PARSING_FATAL_ERR;

    pos = req_line_str.find(' ');
    if (pos == STR::npos || pos == 0)
        return PARSING_STATE::PARSING_FATAL_ERR;

    c.request_.target_ = req_line_str.substr(0, pos);
	unsigned char	ch;
    const STR &t = c.request_.target_;

    // SIZET query_pos = (SIZET)-1;
	STR::size_type query_pos = t.find('?');

    SIZET len = c.request_.target_.size();
    SIZET i = (SIZET)-1;
    while (++i < len)
    {
		ch = static_cast<unsigned char>(t[i]);
        if (ch <= 32 || ch >= 127 || ch == '\\' || ch == '#')
            return PARSING_STATE::PARSING_FATAL_ERR;
        if (t[i] == '%')
        {
            if (i + 2 >= len)
                return PARSING_STATE::PARSING_FATAL_ERR;
            if (!isxdigit(t[i+1]) || !isxdigit(t[i+2]))
                return PARSING_STATE::PARSING_FATAL_ERR;
        }
    }

    if (query_pos != STR::npos)
    {
        c.request_.query_  = c.request_.target_.substr(query_pos + 1);
        c.request_.target_ = c.request_.target_.substr(0, query_pos);
    }

    req_line_str.erase(0, pos + 1);
    c.request_.version_ = req_line_str;
    if (c.request_.version_ != "HTTP/1.1")
        return PARSING_STATE::PARSING_FATAL_ERR;

    return PARSING_STATE::PARSING_OK;
}

#include <stdint.h>
inline static uint8_t hex_to_int(char c)
{
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	return 0;
}

PARSING_STATE::STATE	HTTParser::decode_path(Client &c)
{
    char byt;

    const char *t = c.request_.target_.c_str();
    SIZET len = c.request_.target_.size();

    c.request_.path_tmp_.reserve(len);

    SIZET i = (SIZET)-1;
    while (++i < len)
    {
        if (t[i] == '%')
        {
            byt = (char)((hex_to_int(t[i+1]) << 4) | hex_to_int(t[i+2]));

            if (byt < 32 || byt == 127 || byt == '\\' ||
                byt == '#' || (unsigned char)byt > 127)
                return PARSING_STATE::PARSING_FATAL_ERR;

            c.request_.path_tmp_.push_back(byt);
            i += 2;
        }
        else
            c.request_.path_tmp_.push_back(t[i]);
    }

    return PARSING_STATE::PARSING_OK;
}

PARSING_STATE::STATE	HTTParser::normalize_path(Client &c)
{
    STR tmp;
    std::vector<STR> s;

    c.request_.path_.erase();

    const char *t = c.request_.path_tmp_.c_str();
    SIZET len = c.request_.path_tmp_.size();

    c.request_.path_.reserve(len);

    // detect trailing slash in ORIGINAL path
    bool had_trailing = (len > 0 && t[len - 1] == '/');

    // build segments
    SIZET i = (SIZET)-1;
    while (++i < len)
    {
        if (t[i] == '/')
        {
            // collapse multiple slashes
            while (i + 1 < len && t[i+1] == '/')
                ++i;

            if (tmp.empty()) continue;
            if (tmp == ".") { tmp.erase(); continue; }

            if (tmp == "..")
            {
                if (s.empty())
                    return PARSING_STATE::PARSING_FATAL_ERR;
                s.pop_back();
                tmp.erase();
                continue;
            }

            s.push_back(tmp);
            tmp.erase();
            continue;
        }

        tmp.push_back(t[i]);
    }

    // push last segment if any
    if (!tmp.empty())
        s.push_back(tmp);

    // rebuild normalized path
    len = s.size();
    i = (SIZET)-1;
    while (++i < len)
    {
        c.request_.path_.push_back('/');
        c.request_.path_.append(s[i]);
    }

    // ensure root is "/"
	if (c.request_.path_.empty())
		c.request_.path_ = "/";
	else if (had_trailing && c.request_.path_ != "/")	
		c.request_.path_.push_back('/'); 
    // PRESERVE TRAILING SLASH (except for root)

    return PARSING_STATE::PARSING_OK;
}

