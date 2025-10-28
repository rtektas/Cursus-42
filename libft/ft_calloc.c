/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:36:46 by rtektas           #+#    #+#             */
/*   Updated: 2024/04/19 19:52:52 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*calloc;
	size_t	i;
	size_t	len;

	i = 0;
	len = count * size;
	calloc = malloc(len);
	if (!calloc)
		return (NULL);
	while (i < len)
	{
		calloc[i] = 0;
		i++;
	}
	return (calloc);
}
