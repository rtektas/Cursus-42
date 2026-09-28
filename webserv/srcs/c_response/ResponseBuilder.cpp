/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:51:47 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/29 13:41:21 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <c_response/ResponseBuilder.hpp>
#include <d_client/Client.hpp>
#include <g_utils/Types.hpp>
#include <g_utils/utils.hpp>
#include <g_utils/Logs.hpp>

#include <algorithm>

#include <cctype>
#include <cerrno>
#include <csignal>
#include <cstddef>
#include <ctime>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int	close_fd(int *fd);

template<typename T>
std::string to_string98(const T& value);

BUILD_STATE::STATE	ResponseBuilder::build_response(Client &c)
{
	if (c.request_.is_error_)
		return build_error(c, c.respons_.status_code_);

	const RouteData	&route_data = c.route_data_;

	if (route_data.redirect_code_ != NULL)
		return build_redirect(c);

	if (route_data.meth_locatn_ != NULL)
	{
		const VEC_STR	&methods = *route_data.meth_locatn_;
		if (std::find(methods.begin(), methods.end(), c.request_.method_) == methods.end())
			return build_error(c, 405);
	}

	if (c.route_data_.CGI_locatn_ != NULL)
		if (c.cgi_.parse_interpreter(c) != BUILD_STATE::BUILD_FATAL_ERR)
		{
			Logs::info("DISPATCH CGI path=" + c.request_.path_);
			return build_cgi(c);
		}
	const STR	&method = c.request_.method_;
	if (method == "DELETE")
		return build_delete(c);

	if (method == "POST")
	{
		if (route_data.uplod_store_ != NULL)
			return build_upload(c);
		else
			return build_error(c, 405);	// error 405
	}

	if (method == "GET")
	{
		const STR	*root_ptr = route_data.root_locatn_ != NULL ?
								route_data.root_locatn_ :
								route_data.root_serv_	;
		if (root_ptr == NULL || root_ptr->empty())
			return build_error(c, 500);		// error 500
		const STR	&root = *root_ptr;
		
		c.respons_.full_path_ = root + c.request_.path_;
		STR &full_path = c.respons_.full_path_;

		c.respons_.fd_ = open(full_path.c_str(), O_RDONLY);
		if (c.respons_.fd_ == -1)
		{
			if (errno == ENOENT)
				return build_error(c, 404);
			if (errno == EACCES)
				return build_error(c, 403);
			if (errno == ENAMETOOLONG)
				return build_error(c, 414);
			return build_error(c, 500);
		}

		struct stat	file_stat;

		if (fstat(c.respons_.fd_, &file_stat) == -1)
			{close_fd(&c.respons_.fd_); return build_error(c, 500);}	// 500

		if (S_ISREG(file_stat.st_mode))
		{
			c.respons_.file_stat_ = file_stat;
			if (build_static(c) != BUILD_STATE::BUILD_OK)
				return build_error(c, 500);  
			return BUILD_STATE::BUILD_OK;
		}

		if (S_ISDIR(file_stat.st_mode))
		{
			close_fd(&c.respons_.fd_);

			const STR	*index_file_ptr = route_data.idex_locatn_ != NULL ?
							route_data.idex_locatn_ :
							route_data.idex_serv_	;

			SIZET len = full_path.size();
			if (len > 0 && full_path[len - 1] != '/')
				full_path.push_back('/');

			if (index_file_ptr == NULL)
				full_path += "index.html";
			else
				full_path += *index_file_ptr;

			c.respons_.fd_ = open(full_path.c_str(), O_RDONLY);
			if (c.respons_.fd_ == -1)
			{
				if (errno == ENOENT)
				{
					if (route_data.autoindex_ == true)
						return build_autoindex(c);
					else
						return build_error(c, 404);
				}
				if (errno == EACCES)
					return build_error(c, 403);
				else
					return build_error(c, 500);
			}

			if (fstat(c.respons_.fd_, &file_stat) != -1 && 
				S_ISREG(file_stat.st_mode))
			{
				c.respons_.file_stat_ = file_stat;
				if (build_static(c) != BUILD_STATE::BUILD_OK)
					return build_error(c, 500);  
				return BUILD_STATE::BUILD_OK;
			}
			else
				close_fd(&c.respons_.fd_);

			if (route_data.autoindex_ == true)
				return build_autoindex(c);

			close_fd(&c.respons_.fd_); 
			return build_error(c, 403);		// 403
		}
	}
	return BUILD_STATE::BUILD_FATAL_ERR;
}

#include <time.h>

STR		current_date()
{
	time_t	now = time(NULL);

	struct tm *gmt = gmtime((&now));

	char buf[64];
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmt);

	return buf;
}

#include <c_response/PageError.hpp>

STR get_reason(int code)
{
    if (code == 200) return "OK";
    if (code == 201) return "Created";
    if (code == 204) return "No Content";
    if (code == 301) return "Moved Permanently";
    if (code == 302) return "Found";
	return PageError::getMessage(code);
}

template<typename T>
std::string to_string98(const T& value);

// build w_buf_ from headers + body
void	ResponseBuilder::serialize(Client &c) 
{
	STR		&w_buf = c.w_buf_;

	// build w_buf_ directly
	w_buf = "HTTP/1.1 " + to_string98(c.respons_.status_code_) + " "	+
		get_reason(c.respons_.status_code_)								+ "\r\n" +
		"Date: "				+ current_date()						+ "\r\n" +
		"Content-Type: "		+ c.respons_.content_type_				+ "\r\n" +
		"Content-Length: "		+ to_string98(c.respons_.body_.size())	+ "\r\n" +
		"Connection: "			+ (c.request_.CONN_ ? "close" : "keep-alive")    + 
		"\r\n";

	HEADERS	&headers = c.respons_.headers_;
	SIZET i = -1;
	while (++i < headers.size())
		w_buf += headers[i].key + ": " + headers[i].val + "\r\n";
	w_buf += "\r\n" + c.respons_.body_;

	c.w_pos_ = 0;
}

#include <cstring>
#include <stdlib.h>

void ResponseBuilder::
cgi_serialize_headers(Client &c)
// cgi_serialize_headers(Client &c, STR::size_type headers_end)
{
    const STR &cgi_buf = c.cgi_.cgi_buf_;

    int status_code = 200;
    STR content_type = "text/html";
    STR cookie_val;
    VEC_STR set_cookies;

    // parse CGI headers (same logic as current cgi_serializer)
    char *endptr = NULL;
    long code = 0;
    STR line;
    STR::size_type line_end;
    STR::size_type beg = 0;
    STR::size_type pos = 0;

    // while (pos < headers_end)
	SIZET	size = cgi_buf.size();
	while (pos < size)
    {
        line_end = cgi_buf.find("\n", pos);
		if (line_end == STR::npos)
			break ;

        line = cgi_buf.substr(pos, line_end - pos);

		// strip trailing \r if present (hanldes both \r\n and \n)
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);

		if (line.empty())
			break ;

        if (strncasecmp(line.c_str(), "status:", 7) == 0)
        {
            code = strtol(line.c_str() + 7, &endptr, 10);
            if (endptr != line.c_str() + 7 && code >= 100 && code <= 599)
                status_code = (int)code;
        }
        else if (strncasecmp(line.c_str(), "content-type:", 13) == 0)
        {
            content_type = line.substr(13);
            beg = content_type.find_first_not_of(" \t");
            if (beg != STR::npos)
                content_type.erase(0, beg);
        }
        else if (strncasecmp(line.c_str(), "set-cookie:", 11) == 0)
        {
            cookie_val = line.substr(11);
            beg = cookie_val.find_first_not_of(" \t");
            if (beg != STR::npos)
                cookie_val.erase(0, beg);
            set_cookies.push_back(cookie_val);
        }
        // pos = line_end + 2;
        pos = line_end + 1;
    }

    // build HTTP response headers — no Content-Length, Connection: close
    c.w_buf_ = "HTTP/1.1 " + to_string98(status_code) + " " +
               get_reason(status_code) + "\r\n" +
               "Date: " + current_date() + "\r\n" +
               "Content-Type: " + content_type + "\r\n" +
               "Connection: close\r\n";

    VEC_STR::const_iterator it = set_cookies.begin();
    while (it != set_cookies.end())
    {
        c.w_buf_ += "Set-Cookie: " + *it + "\r\n";
        ++it;
    }
    c.w_buf_ += "\r\n";
    c.w_pos_ = 0;

	// this was moved to allow sending cgi response or big responses in chunks
    // set CONN_ so connection closes after response
    // c.request_.CONN_ = true;
}

// need to handle the error return maybe return BUILD_STATE::STATE
void	ResponseBuilder::cgi_serializer(Client &c)
{
	STR&		w_buf = c.w_buf_;
	const STR&	cgi_buf = c.cgi_.cgi_buf_;
	STR::size_type	headers_end	= cgi_buf.find("\r\n\r\n");
	if (headers_end == STR::npos)
		{ 
			Logs::error("CGI NO HEADER SEPARATOR buf_size=" + to_string98(cgi_buf.size()));
			build_error(c, 500); return;
		}

	// default values;
	int		status_code		= 200;
	STR		content_type	= "text/html";

	STR		cookie_val;

	VEC_STR	set_cookies;

	// parse CGI headers
	char			*endptr = NULL;
	long			code	= 0;
	STR				line;
	STR::size_type	line_end;
	STR::size_type	beg		= 0;
	STR::size_type	pos		= 0;
	while (pos < headers_end)
	{
		 line_end = cgi_buf.find("\r\n", pos);
		 if (line_end == STR::npos || line_end > headers_end)
			 break ;

		 line = cgi_buf.substr(pos, line_end - pos);

		 if (strncasecmp(line.c_str(), "status:", 7) == 0)
		 {
			 code = strtol(line.c_str() + 7, &endptr, 10);
			 if (endptr != line.c_str() + 7 && code >= 100 && code <= 599) 
				 status_code = (int) code;
		 }
		 else
		 if (strncasecmp(line.c_str(), "content-type:", 13) == 0)
		 {
			 content_type = line.substr(13);
			 beg			= content_type.find_first_not_of(" \t");
			 if (beg != STR::npos)
				 content_type.erase(0, beg);
		 }
		 else
		 if (strncasecmp(line.c_str(), "set-cookie:", 11) == 0)
		 {
			 cookie_val = line.substr(11);
			 beg		= cookie_val.find_first_not_of(" \t");
			 if (beg != STR::npos)
				 cookie_val.erase(0, beg);
				// cookie_val = cookie_val.substr(beg);
			 set_cookies.push_back(cookie_val);
		 }
		 pos = line_end + 2;
	}

	// body start after \r\n\r\n
	STR::size_type	body_beg	= headers_end + 4;
	SIZET			body_size	= (body_beg < cgi_buf.size()) ?
									(cgi_buf.size() - body_beg) : 0;

	// build w_buf_ directly (no body_ copy)
	w_buf = "HTTP/1.1 " + to_string98(status_code) + " "				+
			get_reason(status_code)										+ "\r\n"  +
			"Date: "				+ current_date()					+ "\r\n"  +
			"Content-Type: "		+ content_type						+ "\r\n"  +
			"Content-Length: "		+ to_string98(body_size)			+ "\r\n"  +
			"Connection: "			+ (c.request_.CONN_ ? "close" : "keep-alive") + 
			"\r\n";

	VEC_STR::const_iterator const_it		= set_cookies.begin();
	VEC_STR::const_iterator const_it_end	= set_cookies.end();
	while (const_it != const_it_end)
	{
		w_buf += "Set-Cookie: " + *const_it + "\r\n";
		++const_it;
	}

	w_buf += "\r\n";

	if (body_size > 0)
		w_buf.append(cgi_buf, body_beg, body_size);

	c.w_pos_ = 0;
}

BUILD_STATE::STATE	ResponseBuilder::build_redirect(Client &c)
{
	c.respons_.status_code_ = *c.route_data_.redirect_code_;
	STR code	= to_string98(c.respons_.status_code_);
	STR reason	= get_reason(c.respons_.status_code_);

	t_Header	location_header;
	location_header.key = "Location";
	location_header.val	= *c.route_data_.redirect_url_;
	c.respons_.headers_.push_back(location_header);

	c.respons_.body_ = 
		"<html><head><title>" + code + " " + reason + "</title></head>" + 
		"<body><h1>" + code + " " + reason + "</h1><hr>nginx</body></html>";

	c.respons_.content_type_ = "text/html";

	Logs::info("REDIRECT " + to_string98(c.respons_.status_code_) + 
			" -> " + *c.route_data_.redirect_url_);

	serialize(c);

	return BUILD_STATE::BUILD_OK;
}

#include <cstring>

void		Response::reset()
{
	if (fd_ != -1)	close_fd(&fd_);

	status_code_ = 0;
	content_type_.clear();
	body_.clear();
	full_path_.clear();
	memset(&file_stat_, 0, sizeof(file_stat_));

	headers_.clear();

	is_ready_	= false;
	is_chunked_	= false;
	is_error_	= false;

	TYPE_BOUNDARY_VAL_.clear();
}

