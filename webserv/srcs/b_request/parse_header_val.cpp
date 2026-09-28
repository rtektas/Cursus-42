/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header_val.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 11:43:59 by alephoen          #+#    #+#             */
/*   Updated: 2026/05/29 11:43:59 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <b_request/ParsingStructs.hpp>
#include <b_request/HTTParser.hpp>
#include <d_client/Client.hpp>
#include <g_utils/utils.hpp>

#include <limits>

namespace	parse_headers_val
{
	PARSING_STATE::STATE parse_header_unknown_val(STR &val);
	PARSING_STATE::STATE parse_host_val(STR &val);
	PARSING_STATE::STATE parse_CL_val(const STR &str, SIZET &val);
	PARSING_STATE::STATE parse_CT_val(const STR &str);
	PARSING_STATE::STATE parse_TE_val(STR &str, bool &is_chunked);
	PARSING_STATE::STATE parse_Connection_val(STR &str, bool &should_close);
	PARSING_STATE::STATE parse_cookie_val(STR str, STR &val);

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

using namespace	parse_headers_val;

PARSING_STATE::STATE parse_header_val(t_Header &header, 
									 HEADER_NAME::NAME key_name, 
									 HeaderValReturn &ret)
{
	PARSING_STATE::STATE	parse_state;

	switch (key_name)
	{
		case HEADER_NAME::UNKW:
			parse_state = parse_header_unknown_val(header.val);
			if (parse_state == PARSING_STATE::PARSING_FATAL_ERR)
				return (parse_state);
			break ;
		case HEADER_NAME::HOST:
			parse_state = parse_host_val(header.val);
			if (parse_state == PARSING_STATE::PARSING_FATAL_ERR)
				return (parse_state);
			break ;
		case HEADER_NAME::LENT:
			parse_state = parse_CL_val(header.val, ret.LENT_);
			if (parse_state == PARSING_STATE::PARSING_FATAL_ERR)
				return (parse_state);
			break ;
		case HEADER_NAME::TYPE:
			parse_state = parse_CT_val(header.val);
			if (parse_state == PARSING_STATE::PARSING_FATAL_ERR)
				return (parse_state);
			break ;
		case HEADER_NAME::TRNS:
			parse_state = parse_TE_val(header.val, ret.TRNS_);
			if (parse_state == PARSING_STATE::PARSING_FATAL_ERR)
				return (parse_state);
			break ;
		case HEADER_NAME::CONN:
			parse_state = parse_Connection_val(header.val, ret.CONN_);
			if (parse_state == PARSING_STATE::PARSING_FATAL_ERR)
				return (parse_state);
			break ;
		case HEADER_NAME::COOK:
			parse_cookie_val(header.val, ret.COOK_);
			break ;
		default:
			return PARSING_STATE::PARSING_FATAL_ERR;
	}

    return PARSING_STATE::PARSING_OK;
}

PARSING_STATE::STATE 
parse_headers_val::parse_header_unknown_val(STR &val)
{
	unsigned char uc;

	SIZET	size = val.size();
	SIZET	i = -1;
	while (++i < size)
	{
		uc = val[i];
		if (uc == '\t' || uc == ' ')
			continue ;

		if (uc < 32 || uc == 127)
			return PARSING_STATE::PARSING_FATAL_ERR;
	}
    return PARSING_STATE::PARSING_OK;
}

PARSING_STATE::STATE 
parse_headers_val::parse_host_val(STR &val)
{
	if (val.empty())
        return PARSING_STATE::PARSING_FATAL_ERR;

	SIZET	size = val.size();
	char	c = 0;
	SIZET	i  = 0;

	STR::size_type pos_colon = val.find(':');
	if (pos_colon != STR::npos)
	{
		if (pos_colon == size - 1)
			return PARSING_STATE::PARSING_FATAL_ERR;
		i = pos_colon;
		while (++i < size)
		{
			c = static_cast<unsigned char>(val[i]);
			if (!std::isdigit(c))
				return PARSING_STATE::PARSING_FATAL_ERR;
		}
		size = pos_colon;
	}

	if (val[0] == '.' || val[0] == '-')
		return PARSING_STATE::PARSING_FATAL_ERR;

	bool is_DOT = false;
	bool is_HYF = false;
	i = -1;
	while (++i < size)
	{
		c = static_cast<unsigned char>(val[i]);
		if (!isalnum(c) && c != '.' && c != '-')
			return PARSING_STATE::PARSING_FATAL_ERR;
		if (c == '.')
		{
			if (is_HYF || is_DOT)
				return PARSING_STATE::PARSING_FATAL_ERR;
			is_DOT = true;
		}
		else
			is_DOT = false;

		if (c == '-')
		{
			if (is_DOT)
				return PARSING_STATE::PARSING_FATAL_ERR;
			is_HYF = true;
		}
		else
			is_HYF = false;
	}
	if (val[size - 1] == '.' || val[size - 1] == '-')
		return PARSING_STATE::PARSING_FATAL_ERR;

	val = parsing_utils::tolower_case(val, val.size());

    return PARSING_STATE::PARSING_OK;
}

PARSING_STATE::STATE 
parse_headers_val::parse_CL_val(const STR &str, SIZET &val)
{
	if (str.empty())
		return (PARSING_STATE::PARSING_FATAL_ERR);

	unsigned char	c;
	SIZET			d;
	SIZET			n = 0;
	SIZET			size = str.size();
	SIZET			i = -1;
	while (++i < size)
	{
		c = static_cast<unsigned char>(str[i]);
		if (!std::isdigit(c))
			return (PARSING_STATE::PARSING_FATAL_ERR);

		d = c - '0';
		// overflow check (size_t)
		if (n > (std::numeric_limits<size_t>::max() - d) / 10)
			return (PARSING_STATE::PARSING_FATAL_ERR);

		n = (n * 10) + d;
	}

	val = n;

    return PARSING_STATE::PARSING_OK;
}

PARSING_STATE::STATE 
parse_headers_val::parse_CT_val(const STR &str)
{
    VEC_STR	elems;

	STR		type;
	STR		subtype;
	STR		tok;

	SIZET	tok_size;
	SIZET	len = str.size();
	SIZET	i;

	// Content-Type can be treated as absent if non-fatal if val is empty;
	if (!len)
		return PARSING_STATE::PARSING_OK;	

	elems = parsing_utils::vec_split_dble_quoted(str, ';');
	SIZET elems_size = elems.size();
	if (elems_size < 1)
		return (PARSING_STATE::PARSING_FATAL_ERR);

	tok = elems[0];
	tok_size = parsing_utils::trim_str(tok);
	if (!tok_size)
		return PARSING_STATE::PARSING_FATAL_ERR;

	STR::size_type pos; 
	STR::size_type pos_eq; 

	pos = tok.find_first_of(" \t");
	if (pos != STR::npos)
		return PARSING_STATE::PARSING_FATAL_ERR;

	pos = tok.find('/');
	if (pos == STR::npos)
		return PARSING_STATE::PARSING_FATAL_ERR;

	if (pos == tok_size - 1)
		return PARSING_STATE::PARSING_FATAL_ERR;

	pos = tok.find('/', pos + 1);
	if (pos != STR::npos)
		return PARSING_STATE::PARSING_FATAL_ERR;

	i = -1;
	while (++i < tok_size)
	{
		if (tok[i] != '/')
			if (!is_tchar(tok[i]))
				return PARSING_STATE::PARSING_FATAL_ERR;
	}

	SIZET j;
	i = 0;
	while (++i < elems_size)
	{
		tok = elems[i];
		tok_size = parsing_utils::trim_str(tok);
		if (!tok_size)
			return PARSING_STATE::PARSING_FATAL_ERR;

		if (tok[0] == '=' || tok[tok_size - 1] == '=')
			return PARSING_STATE::PARSING_FATAL_ERR;

		pos = tok.find('=');
		pos_eq = pos;
		if (pos != STR::npos)
		{
			pos = tok.find('=', pos + 1);
			if (pos != STR::npos)
				return PARSING_STATE::PARSING_FATAL_ERR;
			pos = pos_eq;
			j = -1;
			while (++j < pos)
			{
				if (!is_tchar(tok[j]))
					return PARSING_STATE::PARSING_FATAL_ERR;
			}
			SIZET sublen = tok_size - pos - 1;
			switch (tok[pos + 1]) 
			{
				case '\"':
				{
					if (sublen == 2 && tok[pos + 2] == '\"')
						break ;

					if (tok[tok_size - 1] != '\"')
						return PARSING_STATE::PARSING_FATAL_ERR;

					if (sublen == 3 && tok[pos + 2] == '\\')
						return PARSING_STATE::PARSING_FATAL_ERR;

					STR tmp = tok.substr(0, pos + 1);
					j = pos + 1;
					while (++j < tok_size - 1)
					{
						if (tok[j] == '"')
							return PARSING_STATE::PARSING_FATAL_ERR;
						if (tok[j] == '\\')
						{
							if (j == tok_size - 2 && tmp[tmp.size() - 1] != '\\')
								return PARSING_STATE::PARSING_FATAL_ERR;
							++j;
							unsigned char c = tok[j];
							if (!(c == '\t' || c == ' ' || 
								c >= 128	|| (33 <= c && c <= 126)))
								return PARSING_STATE::PARSING_FATAL_ERR;
						}
						else if (tok[j] != '\t')
							if (tok[j] < 32   || tok[j] == 127)
								return PARSING_STATE::PARSING_FATAL_ERR;
						tmp.push_back(tok[j]);
					}
					tmp.push_back('\"');
					tok = tmp;
					break ;
				}
				default:
				{
					j = pos;
					while (++j < tok_size)
					{
						if (!is_tchar(tok[j]))
							return PARSING_STATE::PARSING_FATAL_ERR;
					}
				}
			}
		}
		else
		{
			j = -1;
			while (++j < tok_size)
			{
				if (!is_tchar(tok[j]))
					return PARSING_STATE::PARSING_FATAL_ERR;
			}
		}
	}

    return PARSING_STATE::PARSING_OK;
}

PARSING_STATE::STATE 
parse_headers_val::parse_TE_val(STR &str, bool &is_chunked)
{
	if (str.empty())
		return (PARSING_STATE::PARSING_FATAL_ERR);
	str = parsing_utils::tolower_case(str, 7);
	if (str != "chunked")
		return (PARSING_STATE::PARSING_FATAL_ERR);
	is_chunked = true;
    return PARSING_STATE::PARSING_OK;
}

PARSING_STATE::STATE 
parse_headers_val::parse_Connection_val(STR &str, bool &should_close)
{
	bool	close		= false;

	unsigned char	uc;

	if (str.empty())
	{
		should_close = 0;
	    return PARSING_STATE::PARSING_OK;
	}

	VEC_STR	v		= parsing_utils::vec_split_lowercase(str, ',');
	SIZET	vsize	= v.size();

	STR		*tok = NULL;
	SIZET	tok_size;
	SIZET	j;
	SIZET	i = (SIZET)-1;
	while (++i < vsize)
	{
		parsing_utils::trim_str(v[i]);
		tok = &(v[i]);
		tok_size = tok->size();

		j = -1;
		while (++j < tok_size)
		{
			uc = static_cast<unsigned char>((*tok)[j]);
			if (uc < 32 || uc >= 127 || uc == ' ' || uc == '\t')
				return PARSING_STATE::PARSING_FATAL_ERR;
			//(*tok)[j] = std::tolower(uc);	// its already been done in split!
		}
		if (!close)
			if (*tok == "close")
				close = true;
	}

	if (close)
		should_close = 1;
	else
		should_close = 0;

    return PARSING_STATE::PARSING_OK;
}

PARSING_STATE::STATE 
parse_headers_val::parse_cookie_val(STR str, STR &cookies)
{
	if (str.empty())
		return PARSING_STATE::PARSING_FATAL_ERR;

	// SIZET str_len = trim_str(str);	// this is not needed, its already been done!
	SIZET	str_len = str.size();
	if (str_len < 2   ||
		str[0] == '=' ||
		str[0] == '"' ||
		str[0] == ';' ||
		str[str_len - 1] == ';')
		return PARSING_STATE::PARSING_FATAL_ERR;

	bool		semicolon_fnd_in_quote = false;

	VEC_STR		tokens		= 
		parsing_utils::vec_split_err_in_dble_quoted(str, ';', semicolon_fnd_in_quote);

	if (semicolon_fnd_in_quote)
		return PARSING_STATE::PARSING_FATAL_ERR;

	SIZET		tokens_cnt	= tokens.size();
	if (!tokens_cnt)
		return PARSING_STATE::PARSING_FATAL_ERR;

	SIZET			j;

	STR		token;
	SIZET	token_len;

	token = tokens[0];
	token_len = token.size();
	if (token_len < 2 || token[0] == '=')
		return PARSING_STATE::PARSING_FATAL_ERR;

	j = -1;
	while (++j < token_len && token[j] != '=')
	{
		if (!is_tchar(token[j]))
			return PARSING_STATE::PARSING_FATAL_ERR;
	}

	if (j == token_len)			// this means no '=' was found
		return PARSING_STATE::PARSING_FATAL_ERR;

	++j;	// skip the '='
	if (j == token_len - 1 && token[j] == '\"')
		return PARSING_STATE::PARSING_FATAL_ERR;

	//this should be treated for the part after '='
	bool	with_quotes = (j < token_len && token[j] == '\"');
	if (with_quotes)
	{
		if (token[token_len - 1] != '\"')
			return PARSING_STATE::PARSING_FATAL_ERR;
		token_len -= 1;
	}

	while (++j < token_len)
		if (!is_octet(token[j]))
			return PARSING_STATE::PARSING_FATAL_ERR;

	cookies += token;
	if (tokens_cnt > 1)
		cookies += "; ";

	SIZET	i = 0;
	while (++i < tokens_cnt)
	{
		token = tokens[i];
		token_len = token.size();
		if (token_len < 2)
			return PARSING_STATE::PARSING_FATAL_ERR;
		
		if (token[0] != ' ' || token[1] == '=')
			return PARSING_STATE::PARSING_FATAL_ERR;

		j = 0;
		while (++j < token_len && token[j] != '=')
		{
			if (!is_tchar(token[j]))
				return PARSING_STATE::PARSING_FATAL_ERR;
		}

		if (j == token_len)		// this means no '=' was found
			return PARSING_STATE::PARSING_FATAL_ERR;

		++j;	// skip the '='
		if (j == token_len - 1 && token[j] == '\"')
			return PARSING_STATE::PARSING_FATAL_ERR;

		//this should be treated for the part after '='
		with_quotes = (j < token_len && token[j] == '\"');
		if (with_quotes)
		{
			if (token[token_len - 1] != '\"')
				return PARSING_STATE::PARSING_FATAL_ERR;
			token_len -= 1;
		}

		while (++j < token_len)
			if (!is_octet(token[j]))
				return PARSING_STATE::PARSING_FATAL_ERR;

		cookies += token;
		if (i != tokens_cnt - 1)
			cookies += "; ";
	}

    return PARSING_STATE::PARSING_OK;
}

