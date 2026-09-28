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

#ifndef CONVERTION_HPP
#define CONVERTION_HPP

#include <g_utils/Types.hpp>

class Convertion
{
	Convertion();
	~Convertion();

	public :
	static STR getConvertionVectorString(VEC_STR s);
	static STR toLower(const std::string &str);
	static STR toString(SIZET n);
};
#endif
