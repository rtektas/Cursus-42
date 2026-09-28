/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingStructs.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 11:17:37 by alephoen          #+#    #+#             */
/*   Updated: 2026/05/05 11:17:37 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_STRUCTS_HPP
#define PARSING_STRUCTS_HPP

#include <g_utils/Types.hpp>
#include <tr1/unordered_map>

struct	READ_STATE
{
	enum STATE
	{
		READ_OK,
		READ_DONE,
		READ_CLOSE,
		READ_ERROR,
		READ_FATAL
	};
};

struct	PARSING_STATE
{
	enum STATE
	{
		PARSING_OK,
		PARSING_DONE,
		PARSING_WAITING,
		PARSING_FATAL_ERR,
	};
};

struct	CHUNK_BODY_STATE
{
	enum STATE
	{
		CHUNK_SIZE_LINE,
		CHUNK_SIZE_LINE_CR,

		CHUNK_DATA,
		CHUNK_DATA_CR,
		CHUNK_AFTER_DATA,

		CHUNK_TRAILER,
		CHUNK_TRAILER_CR
	};
};

struct	HEADER_STATE
{
	enum STATE
	{
		NEED_MORE_DATA,
		HEADER_COMPLET,
		END_OF_HEADERS,
		HEAD_FATAL_ERR = -1,
	};
};

struct	HEADER_IDX
{
	enum IDX
	{
		HOST = 0,
		LENT = 1,
		TYPE = 2,
		TRNS = 3,
		CONN = 4,
		COOK = 5,
	};
};

struct	STATE_MACHINE
{
	enum	STATE
	{
		NO_CR,
		IS_CR,
		IS_CRLF,
		IS_CRLF_CR,
		IS_CRLF_CRLF,
		STATE_FATAL_ERR = -1
	};
};

struct	HEADER_NAME
{
	enum NAME
	{
		HOST,		// Host
		LENT,		// Content-Length
		TYPE,		// Content-Type
		TRNS,		// Transfer-Encoding
		CONN,		// Connection
		COOK,		// Cookie
		UNKW		// unknown
	};
};

struct HeaderValReturn
{
    SIZET	&LENT_;
    bool	&TRNS_;
    bool	&CONN_;
	STR		&COOK_;

    HeaderValReturn
        (SIZET &len, bool &trns, bool &conn, STR &cook)
        :   LENT_(len), TRNS_(trns), CONN_(conn), COOK_(cook)
    {}
};

typedef struct	RequestLine
{
	STR	method;
	STR	target;
	STR	version;
}	t_RequestLine;

#endif

