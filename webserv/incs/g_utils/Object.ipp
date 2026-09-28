/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logss.ipp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isadbaib <isadbaib@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 20:44:40 by camy              #+#    #+#             */
/*   Updated: 2026/05/27 20:44:40 by camy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_IPP
#define OBJECT_IPP

// Pas besoin de réinclure Object.hpp : ce fichier est inclus DEPUIS Object.hpp

template <typename T>
Value<T>::Value(const T &v) : data(v) {}

template <typename T>
T Value<T>::get() const
{
    return data;
}

template <typename T>
Value<T> Value<T>::operator+(const Value<T> &a) const
{
    return Value<T>(data + a.get());
}

template <typename T>
void Value<T>::print() const
{
    std::cout << data << std::endl;
}

#endif
