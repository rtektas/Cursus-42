/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_CT_response.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:01:09 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/23 17:01:09 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <c_response/ResponseBuilder.hpp>
#include <d_client/Client.hpp>
#include <g_utils/Types.hpp>

#include <cstring>

BUILD_STATE::STATE	ResponseBuilder::parse_CT_response(Client &c)
{
	STR &TYPE = c.request_.TYPE_;

	if (strncasecmp(TYPE.c_str(), "multipart/form-data", 19) != 0)
		return BUILD_STATE::BUILD_FATAL_ERR;

	SIZET	pos = 19;
	while (pos < TYPE.size() && (TYPE[pos] == ' ' || TYPE[pos] == '\t'))
		++pos;

	if ( pos < TYPE.size() && TYPE[pos] != ';')
		return BUILD_STATE::BUILD_FATAL_ERR;

	const char	*ptr = strcasestr(TYPE.c_str(), "boundary=");
	if (ptr == NULL)
		return BUILD_STATE::BUILD_FATAL_ERR;

	STR::size_type	end = (SIZET)-1;
	pos = (ptr + 9) - TYPE.c_str();

	if (pos >= TYPE.size() || TYPE[pos] == ';')	// this means no more char after "boundary="
		return BUILD_STATE::BUILD_FATAL_ERR;

	if (TYPE[pos] == '\"')
	{
		end = TYPE.find('\"', pos+1);
		if (end == STR::npos || end == pos + 1)
			return BUILD_STATE::BUILD_FATAL_ERR;
		--end;
		++pos;
	}
	else
	{
		end = TYPE.find(';', pos);
		if (end != STR::npos)
		{
			--end;
			while (end >= pos && (TYPE[end] == ' ' || TYPE[end] == '\t'))
				--end;
			if (end < pos)
				return BUILD_STATE::BUILD_FATAL_ERR;
		}
		else
		{
			end = TYPE.find_first_of(" \t", pos);
			if (end == STR::npos)
				end = TYPE.size() - 1;
			else
				--end;
		}
	}

	SIZET	size = end - pos + 1;
	if (size > 70)
		return BUILD_STATE::BUILD_FATAL_ERR;
	
	SIZET i = pos - 1;
	while (++i <= end)
	{
		if (
				TYPE[i] == ' '										||
				std::isalnum(static_cast<unsigned char>(TYPE[i]) )	||
				TYPE[i] == '='	|| TYPE[i] == '(' || TYPE[i] == ')'	|| 
				TYPE[i] == '+'  || TYPE[i] == '_' || TYPE[i] == ',' || 
				TYPE[i] == '-'  || TYPE[i] == '.' || TYPE[i] == '/' ||
				TYPE[i] == ':'  || TYPE[i] == '?' || TYPE[i] == '\''
		   )
			continue ;

		return BUILD_STATE::BUILD_FATAL_ERR;
	}

	if (TYPE[end] == ' ')
		return BUILD_STATE::BUILD_FATAL_ERR;

	c.respons_.TYPE_BOUNDARY_VAL_.reserve(72);
	c.respons_.TYPE_BOUNDARY_VAL_ = "--";

	STR &val = c.respons_.TYPE_BOUNDARY_VAL_;
	val += TYPE.substr(pos, size);

	return BUILD_STATE::BUILD_OK;
}

