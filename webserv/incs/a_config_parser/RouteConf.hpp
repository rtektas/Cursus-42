/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConf.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: "alephoen" <"alephoen"@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:18:47 by "alephoen"        #+#    #+#             */
/*   Updated: 2026/06/23 16:18:47 by "alephoen"       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_CONF_HPP
#define ROUTE_CONF_HPP

#include <g_utils/Types.hpp>

#include <cstddef>

class ServersConf;
class ServerConf;

struct RouteData
{
	// ========================== server ==========================
	const STR           *serv_name_;
	const SET_LISTENERS *listeners_;
	const STR           *root_serv_;
	const STR           *idex_serv_;
	const MAP_INT_STR   *eror_page_;
	SIZET				client_max_body_size_serv_;
	// ============================================================

	// ========================== locatn ==========================
	const STR			*rout_locatn_;
	const VEC_STR 		*meth_locatn_;
	const STR     		*root_locatn_;
	const STR     		*idex_locatn_;
	const int     		*redirect_code_;
	const STR     		*redirect_url_;
	bool				autoindex_;
	const STR     		*uplod_store_;
	const VEC_STR		*CGI_locatn_;
	SIZET				client_max_body_size_locatn_;
	// ============================================================

	RouteData()
		: serv_name_(NULL), listeners_(NULL), root_serv_(NULL), idex_serv_(NULL),
		  eror_page_(NULL), client_max_body_size_serv_((SIZET)-1),
		  rout_locatn_(NULL), meth_locatn_(NULL), root_locatn_(NULL), idex_locatn_(NULL),
		  redirect_code_(NULL), redirect_url_(NULL), autoindex_(false),
		  uplod_store_(NULL), CGI_locatn_(NULL), client_max_body_size_locatn_((SIZET)-1)
	{}
};

class RouteConf
{
private:
	ServersConf &servers_conf_;

public:
	RouteConf();
	RouteConf(ServersConf &sc);
	~RouteConf();

	const RouteData		getRouteData
							(int port, 
							 const STR &host_name, 
							 const STR &path)	const;

	SET_LISTENERS		getAllListeners()		const;

	// helpers statiques utilisables par ServerConf et LocationConf
	static VEC_STR collectUntilSemi(const VEC_STR &tokens, SIZET &i, const STR &ctx);
	static SIZET   parseSize(const STR &val);

	template <std::size_t N>
	static bool isOneOf(const STR &val, const STR (&valid)[N])
	{
		for (std::size_t j = 0; j < N; j++)
			if (val == valid[j]) return true;
		return false;
	}
};

#endif

