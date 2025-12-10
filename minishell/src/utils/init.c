/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:55:12 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:26:43 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command	*init_command(const char *word, int type)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->size = 2;
	cmd->argc = 1;
	cmd->argv = malloc(sizeof(char *) * cmd->size);
	if (!cmd->argv)
		return (free(cmd), NULL);
	cmd->argv[0] = ft_strdup(word);
	if (!cmd->argv[0])
		return (free(cmd->argv), free(cmd), NULL);
	cmd->argv[1] = NULL;
	cmd->next_deli = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->heredoc_delim = NULL;
	cmd->next = NULL;
	cmd->pipe_fd[0] = -1;
	cmd->pipe_fd[1] = -1;
	cmd->type = type;
	return (cmd);
}
