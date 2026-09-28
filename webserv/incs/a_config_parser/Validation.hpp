/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validation.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isadbaib <isadbaib@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 15:17:04 by isadbaib          #+#    #+#             */
/*   Updated: 2026/05/04 15:17:04 by isadbaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef VALIDATIOM_HPP
#define VALIDATIOM_HPP

#include <g_utils/Types.hpp>

class Validation
{
    private:

    public :
        Validation();
        ~Validation();
        static bool checkObjet(const STR &obj, const STR &s, size_t i, int depth);
};

#endif

