/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isadbaib <isadbaib@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 00:15:05 by isadbaib          #+#    #+#             */
/*   Updated: 2026/05/31 01:40:31 by isadbaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_CONF_HPP
#define LOCATION_CONF_HPP

#include <a_config_parser/RouteConf.hpp>

typedef std::map<STR, VEC_STR> LOCATION_DIRECTIVES;

class LocationConf
{
private:
	// ─── champs propres a location ──────────────────────────────
	STR          root_;
	VEC_STR      index_;
	VEC_STR      methods_;
	VEC_STR      cgi_extensions_;
	STR          return_directive_;
	int          redirect_code_val_;
	STR          redirect_url_val_;
	STR          upload_store_;
	SIZET        client_max_body_size_;
	bool         autoindex_;
	// ────────────────────────────────────────────────────────────

	STR                  path_;
	VEC_STR              vec_str_;
	LOCATION_DIRECTIVES  directives_;

	void setRoot(const STR &v);
	void setIndex(const VEC_STR &v);
	void setMethods(const VEC_STR &v);
	void setCgiExtensions(const VEC_STR &v);
	void setReturnDirective(const STR &v);
	void setUploadStore(const STR &v);
	void setAutoIndex(bool v);
	void setClientMaxBodySize(SIZET v);

	bool parseDirective(const VEC_STR &tokens, SIZET &i);
	void parseMethods(const VEC_STR &tokens, SIZET &i);
	void parseReturn(const VEC_STR &tokens, SIZET &i);

public:
	LocationConf();
	~LocationConf();

	void ensureDefaultMethods();
	void parse(const STR &path, const VEC_STR &tokens);

	const STR&     getPath()               const;
	const VEC_STR& getDirective(const STR &key) const;

	const STR&        getRoot()              const;
	const VEC_STR&    getIndex()             const;
	const STR&        getFirstIndex()        const;
	const VEC_STR&    getMethods()           const;
	const VEC_STR&    getCgiExtensions()     const;
	const STR&        getReturnDirective()   const;
	const STR&        getUploadStore()       const;
	SIZET             getClientMaxBodySize() const;
	bool              getAutoIndex()         const;
	const int*        getRedirectCodePtr()   const;
	const STR*        getRedirectUrlPtr()    const;
};

#endif

