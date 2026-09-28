/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isadbaib <isadbaib@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 09:00:58 by isadbaib          #+#    #+#             */
/*   Updated: 2026/05/27 00:00:00 by isadbaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONF_HPP
#define SERVER_CONF_HPP

#include <a_config_parser/LocationConf.hpp>
#include <a_config_parser/RouteConf.hpp>
#include <g_utils/Types.hpp>

class ServerConf
{
private:
	// ─── champs propres a server ────────────────────────────────
	STR          root_;
	VEC_STR      index_;
	MAP_INT_STR  error_pages_;
	SIZET        client_max_body_size_;
	bool         autoindex_;
	// ────────────────────────────────────────────────────────────

	VEC_STR           server_name_;
	STR               charset_;
	etag_t            etag_;
	STR               server_tokens_;
	STR               disable_symlinks_;
	VEC_STR           vect_string_config_;
	SET_LISTENERS     listeners_;
	VEC_LOCATION_CONF locations_;
	bool              bool_resu_total_;

	void setRoot(const STR &v);
	void setIndex(const VEC_STR &v);
	void setAutoIndex(bool v);
	void setClientMaxBodySize(SIZET v);
	void addErrorPage(int code, const STR &path);

	bool parseDirective(const VEC_STR &tokens, SIZET &i);
	void parseListen(const VEC_STR &tokens, SIZET &i);
	void parseServerName(const VEC_STR &tokens, SIZET &i);
	void parseCharset(const VEC_STR &tokens, SIZET &i);
	void parseEtag(const VEC_STR &tokens, SIZET &i);
	void parseServerTokens(const VEC_STR &tokens, SIZET &i);
	void parseDisableSymlinks(const VEC_STR &tokens, SIZET &i);
	void parseLocation(const VEC_STR &tokens, SIZET &i);
	void skipDirective(const VEC_STR &tokens, SIZET &i);
	void create();
	void serverClear();

	friend class ServersConf;
	friend class ConfParser;

public:
	ServerConf();
	~ServerConf();

	const STR&             getRoot()              const;
	const VEC_STR&         getIndex()             const;
	const STR&             getFirstIndex()        const;
	const MAP_INT_STR&     getErrorPages()        const;
	SIZET                  getClientMaxBodySize() const;
	bool                   getAutoIndex()         const;

	const VEC_STR&         getServerName()        const;
	const PAIR_LISTENER    getListen()            const;
	const SET_LISTENERS&   getListeners()         const;
	const VEC_LOCATION_CONF& getLocation()        const;
	const VEC_STR          getVectStr()           const;
	bool                   boolNameServer(const STR &name) const;
	const STR&             getPrimaryServerName() const;
};

#endif

