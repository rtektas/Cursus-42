/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:12:28 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/23 17:12:28 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <g_utils/utils.hpp>
#include <g_utils/Types.hpp>



SET_STR parsing_utils::set_split_lower(const STR &str, char delim)
{
    SET_STR	out;
    STR		current;

	SIZET	size = str.size();
    current.reserve(str.size());

	SIZET	i = -1;
    while (++i < size)
    {
        unsigned char c = (unsigned char)str[i];

        if (c == delim)
        {
            if (!current.empty())
            {
                // insert and detect duplicate
                if (!out.insert(current).second)
                    return SET_STR(); // duplicate found
            }
            current.clear();
        }
        else
        {
            // lowercase inline (ASCII only)
            if (c >= 'A' && c <= 'Z')
                c = c + ('a' - 'A');

            current += (char)c;
        }
    }

    // last token
    if (!current.empty())
    {
        if (!out.insert(current).second)
            return SET_STR(); // duplicate found
    }

    return out;
}

VEC_STR parsing_utils::vec_split_multi(const STR &str, const char *delims)
{
    VEC_STR		out;
    STR			current;

	SIZET		size = str.size();

    current.reserve(str.size());

    // Precompute delimiter lookup table (ASCII only)
    bool table[256] = {false};

	SIZET i = -1;
	while (delims[++i])
		table[(unsigned char)delims[i]] = true;

	while (++i < size)
    {
        unsigned char c = (unsigned char)str[i];
        if (table[c])
        {
            out.push_back(current);
            current.clear();
        }
        else
        {
            current += str[i];
        }
    }

    out.push_back(current);
    return out;
}

VEC_STR parsing_utils::vec_split_lowercase(const STR &str, char delim)
{
	VEC_STR		out;
    STR			current;

	SIZET		size = str.size();

    current.reserve(str.size());

	unsigned char	uc;
	SIZET			i = -1;
	while (++i < size)
    {
        uc = static_cast<unsigned char>(str[i]);

        if (uc == delim)
        {
            out.push_back(current);
            current.clear();
        }
        else
        {
            // lowercase ASCII only
            if (uc >= 'A' && uc <= 'Z')
                uc = uc + ('a' - 'A');

            current += static_cast<char>(uc);
        }
    }
    out.push_back(current);
    return out;
}


VEC_STR parsing_utils::vec_split(const STR &str, char delim)
{
	VEC_STR		out;
    STR			current;

	SIZET		size = str.size();

    current.reserve(str.size());

	SIZET			i = -1;
	while (++i < size)
	
    {
        if (str[i] == delim)
        {
            out.push_back(current);
            current.clear();
        }
        else
        {
            current += str[i];
        }
    }

    out.push_back(current); // last token
    return out;
}

SIZET parsing_utils::trim_str(STR &str)
{
    if (str.empty())
        return 0;

    SIZET len   = str.size();
    SIZET start = (SIZET)-1;
    while (++start < len && (str[start] == ' ' || str[start] == '\t'))
        ;

    SIZET end   = str.size();
    while (--end > start && (str[end] == ' ' || str[end] == '\t'))
        ;

    str = str.substr(start, end - start + 1);
    return (str.size());
}


void	parsing_utils::skip_space(const STR &s, SIZET &pos)
{
	SIZET size = s.size();
	while (pos < size)
	{
		if (s[pos] == ' ' || s[pos] == '\t')
			++pos;
		else
			break ;
	}
}

// this doesnt split when inside "" or inside ''
VEC_STR parsing_utils::vec_split_quoted(const STR &str, char delim)
{
    VEC_STR	elems;
    STR		current;
    bool	in_single = false;
    bool	in_double = false;

	STR::const_iterator end	= str.end();
	STR::const_iterator it	= str.begin();
    while (it != end)
    {
        char c = *it;

        if (c == '\'' && !in_double)
		{
            in_single = !in_single;
            current += c;
        }
        else if (c == '"' && !in_single)
		{
            in_double = !in_double;
            current += c;
        }
        else if (c == delim && !in_single && !in_double)
		{
            elems.push_back(current);
            current.clear();
        }
        else
            current += c;
		++it;
    }

    elems.push_back(current);
    return elems;
}

// this doesnt split when inside ""
VEC_STR parsing_utils::vec_split_dble_quoted0(const STR& str, char delim)
{
    VEC_STR elems;
    STR     current;
    bool    in_double = false;   // only track double quotes now

	current.reserve(str.size());

    STR::const_iterator it  = str.begin();
    STR::const_iterator end = str.end();

    while (it != end)
    {
        char c = *it;

        if (c == '"')
		{
            in_double = !in_double;   // toggle only double quotes
            current += c;
        }
        else if (c == delim && !in_double)
		{
            elems.push_back(current);
            current.clear();
        }
        else
            current += c;
        ++it;
    }

    elems.push_back(current);
    return elems;
}

// this doesnt split when inside ""
// this function escapes when we have \" inside ""
VEC_STR parsing_utils::vec_split_dble_quoted(const STR& str, char delim)
{
    VEC_STR elems;
    STR     current;
    bool    in_double = false;

    STR::const_iterator it  = str.begin();
    STR::const_iterator end = str.end();

    while (it != end)
    {
        char c = *it;

        if (c == '"')
        {
            if (in_double && !current.empty() && 
				current[current.size() - 1] == '\\')
            {
                // We are inside quotes and saw \" → treat as normal char "
                // Do NOT toggle in_double, just copy the "
                current += c;
            }
            else
            {
                // Normal quote: toggle in_double and copy it
                in_double = !in_double;
                current += c;
            }
        }
        else if (c == delim && !in_double)
        {
            elems.push_back(current);
            current.clear();
        }
        else
        {
            current += c;
        }

        ++it;
    }

    elems.push_back(current);
    return elems;
}

VEC_STR 
parsing_utils::vec_split_err_in_dble_quoted
				(const STR &str, char delim, bool &error)
{
    VEC_STR elems;
    STR     current;
    bool    in_double = false;   // only track double quotes now

	error = false;

	current.reserve(str.size());

	STR::const_iterator end	= str.end();
	STR::const_iterator it	= str.begin();
    while (it != end)
    {
        char c = *it;

        if (c == '"')
		{
            in_double = !in_double;   // toggle only double quotes
            current += c;
        }
        else if (c == delim && !in_double)
		{
            elems.push_back(current);
            current.clear();
        }
		else if (c == delim && in_double) 
			{ error = true; return VEC_STR(); }
        else
            current += c;
        ++it;
    }

	if (in_double) { error = true; return VEC_STR(); }

    elems.push_back(current);
    return elems;
}


// this doesnt split when inside ""	// not used in webserv
VEC_STR vec_split_dble_quoted_no_escape(STR_CONST_IT it, STR_CONST_IT end, char delim)
{
    VEC_STR elems;
    STR     current;
    bool    in_double = false;   // only track double quotes now

	current.reserve(end - it);

    while (it != end)
    {
        char c = *it;

        if (c == '"')
		{
            in_double = !in_double;   // toggle only double quotes
            current += c;
        }
        else if (c == delim && !in_double)
		{
            elems.push_back(current);
            current.clear();
        }
        else
            current += c;
        ++it;
    }

    elems.push_back(current);
    return elems;
}

// it trims while space around the first char found
SIZET parsing_utils::trim_around_char(STR &str, char ch)
{
	SIZET	len = str.size();
	STR::size_type pos = str.find(ch);
	if (pos == STR::npos)
		return ((SIZET)-1);

	SIZET	cnt_ws_left = 0;
	SIZET	i = pos;
	while (--i < (SIZET)-1 && (str[i] == ' ' || str[i] == '\t'))
		++cnt_ws_left;
	
	SIZET	cnt_ws_rigt = 0;
	i = pos;
	while (++i < len && (str[i] == ' ' || str[i] == '\t'))
		++cnt_ws_rigt;

	STR	tmp;
	tmp.reserve(cnt_ws_left + cnt_ws_rigt + 2);

	tmp = str.substr(0, pos - cnt_ws_left);
	tmp.push_back(ch);
	tmp += str.substr(pos + cnt_ws_rigt + 1, len - (pos + cnt_ws_rigt));

	if (tmp.size() != len)
		str = tmp;

	return (str.size());
}

STR& parsing_utils::tolower_case(STR &str, SIZET size)
{
	SIZET len;
	if (size > str.size())
	    len = str.size();
	else
		len = size;

    SIZET i = (SIZET)-1;
    while (++i < len)
        str[i] = std::tolower(static_cast<unsigned char>(str[i]));
    return (str);
}

#ifndef _INTPTR_T_DEFINED
typedef long intptr_t;
#endif

#include <stdint.h>
#include <unistd.h>

int		close_fd(int *fd)
{
	if (*fd != -1)
	{
		if (close(*fd) == -1)
			return (*fd);
		*fd = -1;
	}
	return (*fd);
}

