/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_autoindex.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:55:39 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/23 16:55:39 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <c_response/ResponseBuilder.hpp>
#include <d_client/Client.hpp>
#include <g_utils/utils.hpp>
// #include <g_utils/Types.hpp>

#include <sys/types.h>
#include <dirent.h>
#include <string.h>

template<typename T>
std::string to_string98(const T& value);

BUILD_STATE::STATE	ResponseBuilder::build_autoindex(Client &c)
{
	const RouteData	&route_data = c.route_data_;

	const STR	*root_ptr = route_data.root_locatn_ != NULL ?
							route_data.root_locatn_ :
							route_data.root_serv_	;
	if (root_ptr == NULL || root_ptr->empty())
		return build_error(c, 500);		// error 500
	const STR	&root = *root_ptr;
	
	c.respons_.full_path_ = root + c.request_.path_;
	STR &full_path = c.respons_.full_path_;

	DIR	*dir	= opendir(full_path.c_str());
	if (dir == NULL)
		return build_error(c, 403);

	c.respons_.body_ = 
		"<html><head><title>Index of " + full_path + 
		"</title></head>" + "<body><h1>Index of " + full_path + 
		"</h1><hr><pre>";

	struct dirent	*entry;
	struct stat		entry_stat;
	struct tm		*tm_info;
	bool			is_dir = false;
	char			time_buf[32];
	STR				size_str;
	STR				name;
	STR				entry_path;
	while ((entry = readdir(dir)) != NULL)
	{
		name = entry->d_name;

		// skip . and ..
		if (name == "." || name == "..")
			continue ;

		// get file info
		entry_path = full_path + "/" + name;
		if (stat(entry_path.c_str(), &entry_stat) == -1)
			continue ;	// skip entries we cannot stat
	
		// append slash for directories
		is_dir = S_ISDIR(entry_stat.st_mode);
		if (is_dir)
		{
			name += "/";
			size_str = "-";
		}
		else
			size_str = to_string98(entry_stat.st_size);

		// format modification time
		tm_info = gmtime(&entry_stat.st_mtime);
		if (tm_info == NULL || 
			strftime(time_buf, sizeof(time_buf), "%d-%b-%Y %H:%M", tm_info) == 0)
			strncpy(time_buf, "unknown", sizeof(time_buf));

		SIZET	padding = name.size() < 50 ? 50 - name.size() : 1;

		// build line
		c.respons_.body_ += 
			"<a href=\"" + name + "\">" + name + "</a>" +
			STR(padding, ' ') + time_buf + "  " +
			size_str + "\n";
	}

	// footer
	c.respons_.body_ += "</pre><hr></body></html>";

	closedir(dir);

	c.respons_.status_code_		= 200;
	c.respons_.content_type_	= "text/html";

	serialize(c);

	return BUILD_STATE::BUILD_OK;
}

