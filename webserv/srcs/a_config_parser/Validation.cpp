/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validation.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isadbaib <isadbaib@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 15:07:09 by camy              #+#    #+#             */
/*   Updated: 2026/05/05 22:45:28 by camy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <a_config_parser/Validation.hpp>
#include <g_utils/Types.hpp>

bool Validation::checkObjet(const STR &obj, const STR &s, std::size_t i = 0, int depth = 0)
{
    if (obj.empty() || obj.size() != 2)
        return (false);
    if (i == s.size())
        return depth == 0; 

    if (s[i] == obj[0])
        return checkObjet(obj, s, i + 1, depth + 1);

    if (s[i] == obj[1]) {
        if (depth == 0)
            return false; 
        return checkObjet(obj,s, i + 1, depth - 1);
    }
    return checkObjet(obj, s, i + 1, depth);
}

