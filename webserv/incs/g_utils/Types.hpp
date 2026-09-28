/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Types.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 19:31:49 by alephoen          #+#    #+#             */
/*   Updated: 2026/04/05 21:06:17 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef TYPES_HPP
#define TYPES_HPP 

#include <cstddef>

#include <sys/types.h>

#include <string>
#include <map>
#include <set>
#include <vector>
#include <utility>

class	LocationConf;
class	ServerConf;

class	Client;

typedef ssize_t						SSIZET;
typedef std::size_t					SIZET;
typedef std::string					STR;

typedef std::pair<int, STR>			PAIR_INT_STR;

typedef std::string::const_iterator STR_CONST_IT ;

#define COUT	std::cout

typedef std::map<int, Client*>		Clients;

typedef std::vector<STR>			VEC_STR;
typedef std::vector<char*>			VEC_CSTR;

typedef std::vector<int>			VEC_INT;

typedef std::set<STR>				SET_STR;

typedef std::pair<STR,int>			PAIR_LISTENER;

typedef std::set<PAIR_LISTENER>		SET_LISTENERS;

// <cgi_pair>        ::= <extension> ":" <interpreter_path>
typedef std::pair<STR,STR>			PAIR_CGI;

typedef std::set<PAIR_CGI>			SET_CGI;

typedef std::vector<LocationConf>	VEC_LOCATION_CONF;

typedef std::vector<ServerConf>		VEC_SERVERS_CONF;

typedef std::map<int, STR>			MAP_INT_STR;	// pages error

typedef	std::map<int, int>			MAP_INT_INT;

typedef struct	Header
{
	STR	key;
	STR	val;

	Header() : key(""), val("") {}
}	t_Header;

typedef std::vector<Header>	HEADERS;

typedef struct s_etag
{
	bool	b;
	STR		s;

	s_etag() : b(true), s("on") {}
}	etag_t;


// ========================================================================
// ServiceUpload limitnginx default1 MB
// AWS API Gateway 10 MB hard limit — cannot be overridden
// Cloudflare 100 MB paid plans
// GitHub file upload 25 MB per file
// Gmail attachment 25 MB
// WhatsApp media16 MB
// ========================================================================

static const SIZET	MAX_READ_BUFFER_SIZE	= 8192;		// 8 KB
static const SIZET	MAX_HEADERS_SUM_SIZE	= 8192;     // 16384;	// 16 KB
static const SIZET	MAX_HEADERS_NUMBER		= 100;

static const SIZET	MAX_BODY_SIZE			= 10 * 1024 * 1024;	// 10 MB hard ceiling
static const SIZET	BODY_MEMORY_THRESHOLD	= 8192;  // 8KB — same as nginx default

static const SIZET MAX_TRAILER_SIZE			= 4096;  // same as nginx

static const SIZET	MAX_ACTIVE_CLIENTS		= 3000;	// max clients pool

static const time_t	CLIENT_TIMEOUT			= 60;	// client will timeout after idle!
static const time_t	CGI_TIMEOUT				= 60;	// client will timeout after idle!

#endif

