/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_error.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:57:30 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/29 13:50:27 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <c_response/ResponseBuilder.hpp>
#include <c_response/PageError.hpp>
#include <d_client/Client.hpp>
#include <g_utils/Logs.hpp>

#include <fcntl.h>

int		close_fd(int *fd);
STR		get_reason(int code);

template<typename T>
std::string to_string98(const T& value);

BUILD_STATE::STATE	ResponseBuilder::build_error(Client &c, int eror_code)
{
	Logs::warn("HTTP ERR " + to_string98(eror_code) + 
			" fd=" + to_string98(c.fd_) + 
			" " + c.request_.method_ + 
			" " + c.request_.path_);

	close_fd(&c.respons_.fd_);

	c.respons_.status_code_ = eror_code;
	const MAP_INT_STR	*eror_page = c.route_data_.eror_page_;

	if (eror_page != NULL)
	{
		const MAP_INT_STR::const_iterator	it = eror_page->find(eror_code);
		if (it != eror_page->end())
		{
			const STR &custom_eror_page_path = it->second;

			const STR *root_ptr =	c.route_data_.root_locatn_	!= NULL ?
									c.route_data_.root_locatn_	:
									c.route_data_.root_serv_	;
			if (root_ptr != NULL)
				c.respons_.full_path_ = *root_ptr + custom_eror_page_path;

			if (read_file_into_body(c) == BUILD_STATE::BUILD_OK)
			{
				c.respons_.status_code_ = 404;
				return BUILD_STATE::BUILD_OK;
			}
		}
	}

	if (eror_code == 405 && c.route_data_.meth_locatn_ != NULL)
	{
		const VEC_STR	&methods = *c.route_data_.meth_locatn_;
		STR				allowed_val;
		SIZET i = -1;
		while (++i < methods.size())
		{
			allowed_val += methods[i];
			if (i < methods.size() - 1)
				allowed_val += ", ";
		}
		t_Header	allowed_header;
		allowed_header.key = "Allow";
		allowed_header.val = allowed_val;
		c.respons_.headers_.push_back(allowed_header);
	}

	c.respons_.content_type_ = "text/html";

	c.respons_.body_ = PageError::getHtml(eror_code);

	serialize(c);

	return BUILD_STATE::BUILD_OK;
}

BUILD_STATE::STATE	ResponseBuilder::read_file_into_body(Client &c)
{
	// open
	c.respons_.fd_ = open(c.respons_.full_path_.c_str(), O_RDONLY);
	if (c.respons_.fd_ == -1)
		return BUILD_STATE::BUILD_NON_FATAL_ERR;

	// fstat
	if (fstat(c.respons_.fd_, &c.respons_.file_stat_) == -1)
	{ close_fd(&c.respons_.fd_); return BUILD_STATE::BUILD_NON_FATAL_ERR; }

	// read
	return build_static(c);
}

