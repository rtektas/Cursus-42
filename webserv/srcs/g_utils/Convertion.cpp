/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isadbaib <isadbaib@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 13:11:03 by isadbaib          #+#    #+#             */
/*   Updated: 2026/05/31 11:38:07 by isadbaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <g_utils/Convertion.hpp>

#include <sstream>
#include <cctype>

STR Convertion::getConvertionVectorString(VEC_STR s)
{
	STR result;
	for (SIZET i = 0; i < s.size(); i++)
	{
		if (i > 0) result += " ";
		result += s[i];
	}
	return result;
}

STR Convertion::toLower(const std::string &str)
{
	STR result = str;
	for (SIZET i = 0; i < result.size(); i++)
		result[i] = std::tolower(result[i]);
	return result;
}

STR Convertion::toString(SIZET n)
{
	std::ostringstream oss;
	oss << n;
	return oss.str();
}

