/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logss.ipp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isadbaib <isadbaib@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 20:44:40 by camy              #+#    #+#             */
/*   Updated: 2026/05/27 20:44:40 by camy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <iostream>

//Classe de base abstraite (non-template -> reste dans le .hpp)
class Object
{
public:
    virtual ~Object() {}
    virtual void print() const = 0;
};

// Déclaration du template
template <typename T>
class Value
{
    T data;
public:
    Value(const T &v);
    T get() const;
    Value<T> operator+(const Value<T> &a) const;
    virtual void print() const;

};

// Inclusion des définitions templates à la fin du header
#include <utils/Object.ipp>

#endif
