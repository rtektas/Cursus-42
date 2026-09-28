/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 09:41:03 by alephoen          #+#    #+#             */
/*   Updated: 2026/04/06 11:19:55 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef UTILS_HPP
#define UTILS_HPP

#include <g_utils/Types.hpp>

#include <iostream>

namespace parsing_utils
{
	// STR			tolower_case(const STR &str, SIZET size);
	STR&		tolower_case(STR &str, SIZET size);

	SET_STR		set_split_lower(const STR &str, char delim);

	VEC_STR		vec_split_multi(const STR &str, const char *delims);
	VEC_STR		vec_split_lowercase(const STR &str, char delim);
	VEC_STR		vec_split(const STR &str, char delim);
	VEC_STR		vec_split_quoted(const STR& str, char delim);
	VEC_STR		vec_split_dble_quoted0(const STR& str, char delim);
	VEC_STR		vec_split_dble_quoted(const STR& str, char delim);
	VEC_STR		vec_split_err_in_dble_quoted
					(const STR &str, char delim, bool &error);

	SIZET		trim_str(STR &str);
	SIZET		trim_around_char(STR &str, char ch);

	void		skip_space(const STR &s, SIZET &pos);
	
	#include <stdint.h>
	inline uint8_t hex_to_int(char c)
	{
		if (c >= '0' && c <= '9') return c - '0';
		if (c >= 'A' && c <= 'F') return c - 'A' + 10;
		if (c >= 'a' && c <= 'f') return c - 'a' + 10;
		return 0;
	}
}

#include <sstream>

template<typename T>
std::string to_string98(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

#endif

