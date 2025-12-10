/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils-parser2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:22:21 by asaracut          #+#    #+#             */
/*   Updated: 2025/11/07 20:08:51 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	skip_word(const char *s, int i)
{
	while (s[i] && !is_blank(s[i]) && !char_len(s + i))
	{
		if (s[i] == '\'' || s[i] == '"')
			i = skip_quote(s, i);
		else
			i++;
	}
	return (i);
}

void	parse_operator(const char *s, int *i)
{
	int	len;

	len = char_len(s + *i);
	*i += len;
}

void	parse_quoted_string(const char *s, int *i)
{
	char	quote;

	quote = s[(*i)++];
	while (s[*i])
	{
		if (s[*i] == quote)
		{
			(*i)++;
			return ;
		}
		else if (s[*i] == '\'' || s[*i] == '"')
			parse_quoted_string(s, i);
		else
			(*i)++;
	}
}

void	parse_word(const char *s, int *i)
{
	while (s[*i] && !is_blank(s[*i]) && !char_len(s + *i))
	{
		if (s[*i] == '\'' || s[*i] == '"')
			parse_quoted_string(s, i);
		else
			(*i)++;
	}
}

int	set_heredoc(t_command **cmd, t_token **cur)
{
	(*cmd)->heredoc = 1;
	if ((*cmd)->heredoc_delim)
		free((*cmd)->heredoc_delim);
	(*cmd)->heredoc_delim = ft_strdup((*cur)->next->value);
	*cur = (*cur)->next;
	return (0);
}
