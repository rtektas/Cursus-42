/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars-redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 03:13:21 by asaracut          #+#    #+#             */
/*   Updated: 2025/11/07 20:20:06 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	set_outfile(t_command **cmd, t_token **cur, int append)
{
	if ((*cmd)->outfile)
		free((*cmd)->outfile);
	(*cmd)->outfile = ft_strdup((*cur)->next->value);
	(*cmd)->append = append;
	*cur = (*cur)->next;
	return (0);
}

int	set_infile(t_command **cmd, t_token **cur)
{
	if ((*cmd)->infile)
		free((*cmd)->infile);
	(*cmd)->infile = ft_strdup((*cur)->next->value);
	*cur = (*cur)->next;
	return (0);
}

static int	create_placeholder(t_command **cmd)
{
	*cmd = init_command("", 5);
	if (!*cmd)
		return (-1);
	return (0);
}

static int	add_pipe_command(t_command **cmd, t_token **cur, int type)
{
	(*cmd)->next = init_command((*cur)->next->value, type);
	if (!(*cmd)->next)
		return (-1);
	*cmd = (*cmd)->next;
	*cur = (*cur)->next;
	return (0);
}

int	guess_delim(t_command **cmd, t_token **cur, int type)
{
	if (!cmd)
		return (-1);
	if (!*cmd && type != 0 && create_placeholder(cmd) == -1)
		return (-1);
	if (!*cmd)
		return (-1);
	if (type == 0)
		return (add_pipe_command(cmd, cur, type));
	if (!(*cur)->next)
		return (-1);
	if (type == 3)
		return (set_infile(cmd, cur));
	if (type == 4 || type == 1)
		return (set_outfile(cmd, cur, type == 1));
	if (type == 2)
		return (set_heredoc(cmd, cur));
	return (-1);
}
