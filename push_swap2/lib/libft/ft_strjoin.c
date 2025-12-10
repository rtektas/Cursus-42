/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:30:31 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:34:03 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	sizes1;
	size_t	sizes2;

	if (!s1 || !s2)
		return (NULL);
	sizes1 = ft_strlen(s1);
	sizes2 = ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * (sizes1 + sizes2 + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, sizes1 + 1);
	ft_strlcat(str, s2, sizes1 + sizes2 + 1);
	return (str);
}
