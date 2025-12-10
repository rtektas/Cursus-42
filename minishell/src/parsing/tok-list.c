/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok-list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 03:13:29 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:26:43 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_command(t_command *cmd)
{
	t_command	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->next_deli)
			free(cmd->next_deli);
		if (cmd->argv)
			ft_free_tab(cmd->argv);
		if (cmd->infile)
			free(cmd->infile);
		if (cmd->outfile)
			free(cmd->outfile);
		if (cmd->heredoc_delim)
			free(cmd->heredoc_delim);
		free(cmd);
		cmd = tmp;
	}
}

int	get_type(const char *tok)
{
	if (!tok || !*tok)
		return (5);
	if (tok[0] == '|' && !tok[1])
		return (0);
	if (tok[0] == '>' && tok[1] == '>' && !tok[2])
		return (1);
	if (tok[0] == '<' && tok[1] == '<' && !tok[2])
		return (2);
	if (tok[0] == '<' && !tok[1])
		return (3);
	if (tok[0] == '>' && !tok[1])
		return (4);
	if (tok[0] == '\'')
		return (9);
	if (tok[0] == '"')
		return (10);
	return (5);
}

static t_token	*new_token(const char *str)
{
	t_token	*tok;
	char	*val;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	val = clean_token(str);
	if (!val)
		return (free(tok), NULL);
	tok->value = val;
	tok->type = get_type(str);
	tok->next = NULL;
	return (tok);
}

static int	append_token(t_token **cur, const char *value)
{
	(*cur)->next = new_token(value);
	if (!(*cur)->next)
		return (0);
	*cur = (*cur)->next;
	return (1);
}

t_token	*build_list(char **tab)
{
	t_token	*head;
	t_token	*cur;
	int		i;

	if (!tab || !tab[0])
		return (NULL);
	head = new_token(tab[0]);
	if (!head)
		return (NULL);
	cur = head;
	i = 1;
	while (tab[i])
	{
		if (!append_token(&cur, tab[i]))
		{
			free_token_list(head);
			return (NULL);
		}
		i++;
	}
	return (head);
}
