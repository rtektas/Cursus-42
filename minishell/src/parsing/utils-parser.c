/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils-parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:17:26 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:26:43 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_blank(char c)
{
	return (c == ' ' || c == '\t');
}

int	skip_blanks(const char *s, int i)
{
	while (is_blank(s[i]))
		i++;
	return (i);
}

void	skip_blanks_with_ptr(const char *s, int *i)
{
	while (is_blank(s[*i]))
		(*i)++;
}

int	char_len(const char *str)
{
	if ((str[0] == '<' && str[1] == '<')
		|| (str[0] == '>' && str[1] == '>'))
		return (2);
	if (str[0] == '<' || str[0] == '>'
		|| str[0] == '|')
		return (1);
	return (0);
}

int	skip_quote(const char *s, int i)
{
	char	quote;

	quote = s[i++];
	while (s[i])
	{
		if (s[i] == quote)
			return (i + 1);
		else if (s[i] == '\'' || s[i] == '"')
			i = skip_quote(s, i);
		else
			i++;
	}
	return (i);
}
