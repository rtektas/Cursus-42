/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_upload.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:58:17 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/29 13:44:42 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <c_response/ResponseBuilder.hpp>
#include <d_client/Client.hpp>
#include <g_utils/Logs.hpp>

#include <cerrno>
#include <cstring>
#include <fcntl.h>

int			close_fd(int *fd);

template<typename T>
std::string to_string98(const T& value);

bool	is_file_name_valid(const char *beg, const char *end, STR &str)
{
	SIZET	cnt = 0;

	if (!beg || !end || *beg == '.') return (0);

	const char	*ptr = beg;
	while (++cnt < 255 && ptr < end)
	{
		if (*ptr == '\0' || *ptr == '/' || *ptr == '\\')
			return (0);
		if (*ptr == '.' && *(ptr + 1) == '.')
			return (0);
		if (*ptr == '.' || *ptr == '_' ||  *ptr == '-' || 
			isalnum(static_cast<unsigned char>(*ptr)))
			{ str.push_back(*ptr); ++ptr; continue ;}
		return (0);
	}
	if (*end == '.' || *end == '_' || *end == '-' || 
		isalnum(static_cast<unsigned char>(*end)))
		{ str.push_back(*end); return (1); }
	return (0);
}

inline bool	add_validated_char_2_file_name(const char *ptr, STR &str)
{
	if (*ptr == '\0' || *ptr == '/' || *ptr == '\\')
		return (0);
	if (*ptr == '.' && *(ptr + 1) == '.')
		return (0);
	if (*ptr == '.' || *ptr == '_' ||  *ptr == '-' || 
		isalnum(static_cast<unsigned char>(*ptr)))
		{ str.push_back(*ptr); return (1) ;}
	return (0);
}

BUILD_STATE::STATE	ResponseBuilder::build_upload(Client &c)
{
	// =======================================================================
	//							parsing Content-Type
	// =======================================================================
	if (parse_CT_response(c) != BUILD_STATE::BUILD_OK)
		return build_upload_raw_file(c);
		// return build_error(c, 400);
	// =======================================================================

	// =======================================================================
	//							Reading body into memory
	// =======================================================================
	char	buf[4096];
	SSIZET	byts_r;
	int		fd = c.request_.body_file_fd_;
	if (fd != -1)
	{
		c.respons_.body_.reserve(c.request_.body_size_);
		lseek(fd, 0, SEEK_SET);
		while (true)
		{
			byts_r = read(fd, buf, sizeof(buf));
			if (byts_r == -1)
				return BUILD_STATE::BUILD_FATAL_ERR;
			if (byts_r == 0)
				{ close_fd(&c.request_.body_file_fd_); break ;}
			c.respons_.body_.append(buf, byts_r);
		}
	}

	// if the local fd == -1 it means that the body is already in request_.body_
	STR	&body = (fd != -1) ? c.respons_.body_ : c.request_.body_;
	// =======================================================================

	// =======================================================================
	//				Retrieving boundry and advancing the point
	// =======================================================================
	STR &boundry	= c.respons_.TYPE_BOUNDARY_VAL_;

	STR::size_type	pos = body.find(boundry);
	if (pos == STR::npos)
		return build_error(c, 400);
	pos += boundry.size();

	if (pos + 1 < body.size() && (body[pos] != '\r' || body[pos+1] != '\n'))
		return build_error(c, 400);
	pos += 2;
	// =======================================================================

	// =======================================================================
	//					Parsing the File Name
	// =======================================================================
	STR upload_path;
	upload_path.reserve((*c.route_data_.uplod_store_).size() + 50);

	if ((*c.route_data_.uplod_store_)[c.route_data_.uplod_store_->size() - 1] == '/')
		upload_path = *c.route_data_.uplod_store_; //+ "/";	
	else
		upload_path = *c.route_data_.uplod_store_ + "/";	

	const char	*cd_ptr = 
		strcasestr(body.c_str() + pos, "Content-Disposition:");
	if (cd_ptr == NULL)
		return build_error(c, 400);

	const char	*fn_ptr = strcasestr(cd_ptr, "filename=");
	if (fn_ptr == NULL)
		return build_error(c, 400);

	// const char	*fn_ptr = ""

	SIZET	fn_pos = (fn_ptr + 9) - body.c_str();
	if (fn_pos >= body.size())
		return build_error(c, 400);

	SIZET	i = fn_pos;
	if (body[i] == '\"') 
	{
		++i;
		STR::size_type end = body.find('\"', i);
		if (end == STR::npos)
			return build_error(c, 400);

		if (i >= end)
			return build_error(c, 400);

		if (!is_file_name_valid(&(body[i]), &(body[end - 1]), upload_path))
			return build_error(c, 400);
	}
	else
	{
		if (body[i] == '.' || body[i] == ' ' || 
			body[i] == '\t' || body[i] == ';')
			return build_error(c, 400);

		--i;
		SIZET cnt = 0;
		SIZET size = body.size();
		while (++i < size - 1 && body[i] != ';' && 
			   body[i] != ' ' && body[i] != '\t' && body[i] != '\r')
		{
			if (++cnt > 255)
				return build_error(c, 400);
			if (!add_validated_char_2_file_name(&(body[i]), upload_path))
				return build_error(c, 400);

		}
		if (i == size - 1)
		{
			if (body[i] == '.' || body[i] == '_' || body[i] == '-' || 
				isalnum(static_cast<unsigned char>(body[i])))
					upload_path.push_back(body[i]);
			else
				return build_error(c, 400);
		}
	}
	// =======================================================================

	// =======================================================================
	//							Find Content Start
	// =======================================================================
	STR::size_type beg = body.find("\r\n\r\n", pos);
	if (beg == STR::npos)
		return build_error(c, 400);
	// =======================================================================
	
	// =======================================================================
	//							Find Content Start
	// =======================================================================
	STR::size_type end = body.find("\r\n" + boundry, beg);

	if (end == STR::npos) 
		return build_error(c, 400);

	if (end + 2+ boundry.size() > body.size())
			return build_error(c, 400);

	if (strncmp(&(body[end+2]), boundry.c_str(), boundry.size()) != 0)
		return build_error(c, 400);

	beg += 4;
	// =======================================================================
	
	// =======================================================================
	//							Write file
	// =======================================================================
	int	w_fd = open(upload_path.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0644);
	if (w_fd == -1)
	{
		if (errno == EEXIST)
		{
			Logs::warn("UPLOAD CONFLICT " + upload_path);
			return build_error(c, 409);
		}
		if (errno == EACCES)
			return build_error(c, 403);
		else
			return build_error(c, 500);
	}
	
	SSIZET byts_w = write(w_fd, body.c_str() + beg, end - beg);
	if (byts_w == -1 || byts_w < static_cast<SSIZET>(end - beg))
	{
		close_fd(&w_fd); unlink(upload_path.c_str()); return build_error(c, 500);
	}
	close_fd(&w_fd);
	Logs::info("UPLOADED " + upload_path + " size=" + to_string98(end - beg));
	// =======================================================================
	
	// =======================================================================
	//							Build Response
	// =======================================================================
	c.respons_.status_code_		= 201;
	c.respons_.body_			= "";
	c.respons_.content_type_	= "text/html";

	serialize(c);

	return BUILD_STATE::BUILD_OK;
	// =======================================================================
}

#include <stdlib.h>

static int		open_new_file_with_random_name(STR &path);

BUILD_STATE::STATE	ResponseBuilder::build_upload_raw_file(Client &c)
{
	// =======================================================================
	//							Reading body into memory
	// =======================================================================
	char	buf[4096];
	SSIZET	byts_r;
	int		fd = c.request_.body_file_fd_;
	if (fd != -1)
	{
		c.respons_.body_.reserve(c.request_.body_size_);
		lseek(fd, 0, SEEK_SET);
		while (true)
		{
			byts_r = read(fd, buf, sizeof(buf));
			if (byts_r == -1)
				return BUILD_STATE::BUILD_FATAL_ERR;
			if (byts_r == 0)
				{ close_fd(&c.request_.body_file_fd_); break ;}
			c.respons_.body_.append(buf, byts_r);
		}
	}

	STR	&body = (fd != -1) ? c.respons_.body_ : c.request_.body_;
	// =======================================================================

	// =======================================================================
	//							OPEN file
	// =======================================================================
	STR upload_path;
	upload_path.reserve((*c.route_data_.uplod_store_).size() + 50);

	if ((*c.route_data_.uplod_store_)[c.route_data_.uplod_store_->size() - 1] == '/')
		upload_path = *c.route_data_.uplod_store_; //+ "/";	
	else
		upload_path = *c.route_data_.uplod_store_ + "/";	
	// =======================================================================

	// =======================================================================
	//							Write file
	// =======================================================================
	upload_path += "uploaded_raw_file_XXXXXX";
	int	w_fd = open_new_file_with_random_name(upload_path);
	if (w_fd == -1)
	{
		if (errno == EACCES)
			return build_error(c, 403);
		else
			return build_error(c, 500);
	}

	SSIZET byts_w = write(w_fd, body.c_str(), body.size());
	if (byts_w == -1 || byts_w < static_cast<SSIZET>(body.size()))
	{
		close_fd(&w_fd); unlink(upload_path.c_str()); return build_error(c, 500);
	}
	close_fd(&w_fd);
	Logs::info("RAW UPLOAD " + upload_path + " size=" + to_string98(body.size()));
	// =======================================================================
	
	// =======================================================================
	//							Build Response
	// =======================================================================
	c.respons_.status_code_		= 201;
	c.respons_.body_			= "";
	c.respons_.content_type_	= "text/html";

	serialize(c);

	return BUILD_STATE::BUILD_OK;
	// =======================================================================
}

static int		open_new_file_with_random_name(STR &path)
{

	// STR file_path = path + "_XXXXXX"; it should receive the _XXXXXX
	std::vector<char> tmp(path.begin(), path.end());
	tmp.push_back('\0');

	int	fd = mkstemp(tmp.data());
	if (fd == -1)
		return (-1);

	path = tmp.data();

	return (fd);
}

