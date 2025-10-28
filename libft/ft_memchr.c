/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:32:36 by rtektas           #+#    #+#             */
/*   Updated: 2024/04/15 18:02:58 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;

	str = (unsigned char *)s;
	while (n--)
	{
		if (*str != (unsigned char)c)
			str++;
		else
			return (str);
	}
	return (0);
}

// int main()
// {
// char s[] = {0, 1, 2 ,3 ,4 ,5};

// ft_memchr(s, 0, 0);
// ft_memchr(s, 2, 3);
// }
