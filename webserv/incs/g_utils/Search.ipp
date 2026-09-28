/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Earch.ipp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isadbaib <isadbaib@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 20:43:52 by camy              #+#    #+#             */
/*   Updated: 2026/05/27 20:43:52 by camy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEARCH_IPP
#define SEARCH_IPP

#include <algorithm>
#include <vector>

template <typename T>
int earch(std::vector<T> l, const T &cible)
{
	typename std::vector<T>::iterator it = std::find(l.begin(), l.end(), cible);
	(void)it;
	if (it != l.end())
		return (it - l.begin());
	else
		return (-1);
}

#endif

