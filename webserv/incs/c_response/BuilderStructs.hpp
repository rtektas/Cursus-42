/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BuilderStructs.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: "alephoen" <"alephoen"@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:20:23 by alephoen          #+#    #+#             */
/*   Updated: 2026/06/23 16:20:23 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILDER_STRUCTS_HPP
#define BUILDER_STRUCTS_HPP

#include <g_utils/Types.hpp>

struct	WRIT_STATE
{
	enum STATE
	{
		WRIT_OK,
		WRIT_CGI,
		WRIT_DONE,
		WRIT_CLOSE,
		WRIT_ERROR,
		WRIT_FATAL
	};
};

struct	BUILD_STATE
{
	enum STATE
	{
		BUILD_OK,
		BUILD_FATAL_ERR,
		BUILD_NON_FATAL_ERR,
		BUILD_CGI_RUNNING
	};
};

#endif

