/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 18:03:42 by rtektas           #+#    #+#             */
/*   Updated: 2024/04/19 19:59:15 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*t_dst;
	const char	*t_src;

	t_dst = (char *)dst;
	t_src = (const char *)src;
	if (!dst && !src)
		return (dst);
	while (n--)
		*t_dst++ = *t_src++;
	return (dst);
}

/*
int	main(void)
{

	char src[] = "/0";
char	dest[6];
ft_memcpy(dest, src, sizeof(src));

int		numbers[] = {1, 2, 3, 4, 5};
int		copy[5];
ft_memcpy(copy, numbers, 5 * sizeof(int));
}*/
