/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 03:13:21 by asaracut          #+#    #+#             */
/*   Updated: 2025/11/07 20:25:15 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* add_arg moved to parser_helpers.c */

static int	add_word(t_command **cmd, t_token *cur, t_command **head)
{
	if (!*cmd)
	{
		*cmd = init_command(cur->value, cur->type);
		if (!*cmd)
			return (-1);
		if (!*head)
			*head = *cmd;
	}
	else if (add_arg(*cmd, cur->value) < 0)
		return (-1);
	return (0);
}

static int	dispatch(t_command **cmd, t_token **cur, t_command **head)
{
	if ((*cur)->type == 5 || (*cur)->type == 9 || (*cur)->type == 10)
		return (add_word(cmd, *cur, head));
	if ((*cur)->type >= 0 && (*cur)->type <= 4)
		return (guess_delim(cmd, cur, (*cur)->type));
	return (0);
}

static void	init_parse_vars(t_token *tok, t_token **cur, t_command **cmd,
		t_command **head)
{
	*cur = tok;
	*cmd = NULL;
	*head = NULL;
}

t_command	*ft_create_struct_command(t_token *tok)
{
	t_token		*cur;
	t_command	*cmd;
	t_command	*head;

	init_parse_vars(tok, &cur, &cmd, &head);
	while (cur)
	{
		if (dispatch(&cmd, &cur, &head) < 0)
			return (NULL);
		if (!head && cmd)
			head = cmd;
		cur = cur->next;
	}
	if (cmd)
		cmd->argv[cmd->argc] = NULL;
	if (head)
		return (head);
	return (cmd);
}

void	parse_token(t_token *toklist, t_minishell **mini)
{
	t_command	*cmd;

	cmd = ft_create_struct_command(toklist);
	if (!cmd)
		return ;
	(*mini)->cmd = count_cmds(cmd);
	handle_exec(cmd, mini);
	free_command(cmd);
}
