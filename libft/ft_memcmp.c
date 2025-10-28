/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 18:05:08 by rtektas           #+#    #+#             */
/*   Updated: 2024/04/19 20:03:28 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*str;
	const unsigned char	*str1;

	str = s1;
	str1 = s2;
	while (n--)
	{
		if (*str != *str1)
			return (*str - *str1);
		str++;
		str1++;
	}
	return (0);
}
