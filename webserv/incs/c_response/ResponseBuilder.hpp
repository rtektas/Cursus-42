/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 20:43:26 by alephoen          #+#    #+#             */
/*   Updated: 2026/05/04 20:43:26 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_BUILDER_HPP
#define RESPONSE_BUILDER_HPP

#include <c_response/Response.hpp>
#include <c_response/BuilderStructs.hpp>

class client;
class Response;

class	ResponseBuilder
{
private:
	BUILD_STATE::STATE	build_static(Client &c);
	BUILD_STATE::STATE	build_error(Client &c, int eror_code);
	BUILD_STATE::STATE	build_redirect(Client &c);
	BUILD_STATE::STATE	build_autoindex(Client &c);
	BUILD_STATE::STATE	build_delete(Client &c);
	BUILD_STATE::STATE	build_upload(Client &c);
	BUILD_STATE::STATE	build_cgi(Client &c);

	BUILD_STATE::STATE	parse_CT_response(Client &c);
	BUILD_STATE::STATE	build_upload_raw_file(Client &c);

	void				serialize(Client &c);		// build w_buf_ from headers + body

	void				cgi_serializer(Client &c);	// this is replaced be the one below

	void				cgi_serialize_headers(Client &c);

	BUILD_STATE::STATE	read_file_into_body(Client &c);

	friend class Client;
	friend class Response;
	friend class CGIHandler;

public:
	BUILD_STATE::STATE	build_response(Client &c);

};

#endif

