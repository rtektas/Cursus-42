/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok-split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 03:13:32 by asaracut          #+#    #+#             */
/*   Updated: 2025/11/11 16:30:54 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_token(const char *s)
{
	int	i;
	int	len;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		i = skip_blanks(s, i);
		if (!s[i])
			break ;
		len = char_len(s + i);
		if (len)
			i += len;
		else
			i = skip_word(s, i);
		count++;
	}
	return (count);
}

char	*clean_token(const char *str)
{
	if (!str)
		return (NULL);
	return (ft_strdup(str));
}

char	*extract_token(const char *s, int *i)
{
	int		start;
	char	*token;

	skip_blanks_with_ptr(s, i);
	start = *i;
	if (char_len(s + *i))
		parse_operator(s, i);
	else
		parse_word(s, i);
	token = ft_substr(s, start, *i - start);
	if (!token)
		return (NULL);
	return (token);
}

char	**ft_split_tokens(char *s)
{
	char	**tab;
	int		count;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	count = count_token(s);
	tab = malloc(sizeof(char *) * (count + 1));
	if (!tab)
		return (NULL);
	i = 0;
	j = 0;
	while (j < count)
	{
		tab[j] = extract_token(s, &i);
		if (!tab[j])
			return (ft_free_split(tab), NULL);
		j++;
	}
	tab[j] = NULL;
	return (tab);
}
