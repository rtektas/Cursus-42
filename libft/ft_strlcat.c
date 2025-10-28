/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 18:57:49 by rtektas           #+#    #+#             */
/*   Updated: 2024/04/23 17:48:42 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t n)
{
	size_t		dest_len;
	size_t		total_len;
	const char	*s;

	if ((!dest || !src) && !n)
		return (0);
	s = src;
	dest_len = 0;
	while (*(dest + dest_len) && dest_len < n)
		dest_len++;
	if (dest_len < n)
		total_len = dest_len + ft_strlen(s);
	else
		return (n + ft_strlen(s));
	while (*s && (dest_len + 1) < n)
	{
		*(dest + dest_len) = *s++;
		dest_len++;
	}
	*(dest + dest_len) = '\0';
	return (total_len);
}
