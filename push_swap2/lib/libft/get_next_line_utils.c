/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:35:33 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:26:43 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

char	*ft_strdup_gnl(char *s)
{
	char	*str;
	size_t	i;
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str || !s)
	{
		if (str)
			ft_free_gnl(str);
		if (s)
			ft_free_gnl(s);
		return (NULL);
	}
	i = 0;
	while (i < len)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*str;
	size_t	i;
	size_t	j;

	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)) + 1);
	if (!str)
		return (ft_free_gnl(s1));
	i = -1;
	j = 0;
	if (!s1)
	{
		ft_free_gnl(str);
		return (ft_strdup_gnl(s2));
	}
	while (s1[++i])
		str[i] = s1[i];
	while (s2[j])
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	ft_free_gnl(s1);
	return (str);
}

char	*ft_strchr_gnl(const char *s, int c)
{
	int					i;
	unsigned char		ca;
	char				*str;

	ca = (unsigned char)c;
	str = (char *)s;
	i = 0;
	if (!s)
		return (NULL);
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

void	*ft_free_gnl(char *str)
{
	if (str)
		free(str);
	str = NULL;
	return (NULL);
}
