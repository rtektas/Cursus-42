/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PageError.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <rtektas@student.42belgium.be>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 20:42:58 by rtektas           #+#    #+#             */
/*   Updated: 2026/05/27 20:42:58 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PAGE_ERROR_HPP
#define PAGE_ERROR_HPP

#include <g_utils/Types.hpp>

struct ErrorEntry
{
    int         code;
    const char *message;
};

class PageError
{
public:
    static STR        getHtml(int code);
    static STR        getHtml(int code, const STR &custom_path);
    static const STR  getMessage(int code);
    static bool       isKnown(int code);

    // tableau de tous les codes HTTP supportes
    static const ErrorEntry ERROR_TABLE[];
};

#endif

