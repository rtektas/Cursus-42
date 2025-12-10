/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:30:31 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:33:46 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

static char	**ft_free(char **str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

static int	ft_count_strings(const char *str, char sep)
{
	int			i;
	int			count;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && str[i] == sep)
			i++;
		if (str[i])
			count++;
		while (str[i] && !(str[i] == sep))
			i++;
	}
	return (count);
}

static char	*ft_substr_split(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	s_len;

	if (!s || start >= ft_strlen(s))
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
	{
		free(str);
		return (NULL);
	}
	ft_strlcpy(str, s + start, len + 1);
	return (str);
}

char	**ft_split(char const *s, char c)
{
	t_split	var;

	var.i = 0;
	if (!s)
		return (NULL);
	var.tab = (char **)malloc((ft_count_strings(s, c) + 1) * sizeof(char *));
	if (!var.tab)
		return (NULL);
	while (*s)
	{
		if (*s != c)
		{
			var.len = 0;
			while (s[var.len] && s[var.len] != c)
				var.len++;
			var.tab[var.i++] = ft_substr_split(s, 0, var.len);
			if (var.tab[var.i - 1] == NULL)
				return (ft_free(var.tab));
			s += var.len;
		}
		else
			s++;
	}
	var.tab[var.i] = NULL;
	return (var.tab);
}
