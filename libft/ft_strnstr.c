/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 18:16:22 by rtektas           #+#    #+#             */
/*   Updated: 2024/04/22 17:52:19 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	int		length;
	char	*large;
	char	*small;

	if (!big && len == 0)
		return (NULL);
	if (*little == '\0')
		return ((char *)big);
	large = (char *)big;
	small = (char *)little;
	i = 0;
	length = ft_strlen(small);
	while (large[i] != '\0' && (i + length) <= len)
	{
		if (ft_strncmp((large + i), small, length) == 0)
		{
			return (large + i);
		}
		i++;
	}
	return (NULL);
}
