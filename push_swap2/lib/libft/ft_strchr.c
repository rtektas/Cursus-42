/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:30:31 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:33:50 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

char	*ft_strchr(const char *s, int c)
{
	int					i;
	unsigned char		ca;
	char				*str;

	ca = (unsigned char)c;
	str = (char *)s;
	i = 0;
	while (str[i])
	{
		if (str[i] == ca)
			return ((char *)&str[i]);
		i++;
	}
	if (str[i] == ca)
		return ((char *)&str[i]);
	return (NULL);
}
