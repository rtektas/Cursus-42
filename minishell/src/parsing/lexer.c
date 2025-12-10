/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 03:13:14 by asaracut          #+#    #+#             */
/*   Updated: 2025/11/11 16:31:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_clear_tokens(t_token **lst)
{
	t_token	*tmp;

	if (!lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free((*lst)->value);
		free(*lst);
		*lst = tmp;
	}
}

static int	is_operator(int type)
{
	if (type == 0 || type == 1 || type == 2 || type == 3 || type == 4)
		return (1);
	return (0);
}

static int	is_pipe_error(t_token *cur)
{
	if (cur->type == 0)
	{
		if (!cur->next || cur->next->type == 0 || (cur->next->type >= 1
				&& cur->next->type <= 4))
			return (ft_printf("syntax error near unexpected token `|`\n"), 1);
	}
	if (cur->type == 1 || cur->type == 2 || cur->type == 3 || cur->type == 4)
	{
		if (!cur->next)
			return (ft_printf("syntax error near unexpected token `newline`\n"),
				1);
		if (is_operator(cur->next->type))
			return (ft_printf("syntax error near unexpected token `%s`\n",
					cur->next->value), 1);
	}
	return (0);
}

static int	syntax_error(t_token *lst)
{
	t_token	*cur;

	if (!lst)
		return (1);
	cur = lst;
	while (cur)
	{
		if (is_pipe_error(cur))
			return (1);
		cur = cur->next;
	}
	return (0);
}

t_token	*lexer(char *prompt)
{
	char	**tab;
	t_token	*tokens;

	if (!prompt)
		return (NULL);
	tab = ft_split_tokens(prompt);
	if (!tab)
		return (NULL);
	tokens = build_list(tab);
	ft_free_split(tab);
	if (!tokens || syntax_error(tokens))
	{
		ft_clear_tokens(&tokens);
		return (NULL);
	}
	return (tokens);
}
