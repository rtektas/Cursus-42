/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 03:13:25 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:26:43 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	redirect_input(t_command *cmd)
{
	int	fd;

	if (!cmd->infile)
		return (0);
	fd = open(cmd->infile, O_RDONLY);
	if (fd < 0)
		return (perror(cmd->infile), 1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror("dup2 infile"), close(fd), 1);
	close(fd);
	return (0);
}

static int	redirect_output_trunc(t_command *cmd)
{
	int	fd;

	if (!cmd->outfile || cmd->append)
		return (0);
	fd = open(cmd->outfile,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(cmd->outfile), 1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (perror("dup2 outfile"), close(fd), 1);
	close(fd);
	return (0);
}

static int	redirect_output_append(t_command *cmd)
{
	int	fd;

	if (!cmd->outfile || !cmd->append)
		return (0);
	fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror(cmd->outfile), 1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (perror("dup2 append"), close(fd), 1);
	close(fd);
	return (0);
}

static int	redirect_heredoc(t_command *cmd)
{
	if (!cmd->heredoc)
		return (0);
	if (cmd->pipe_fd[1] != -1)
		close(cmd->pipe_fd[1]);
	if (dup2(cmd->pipe_fd[0], STDIN_FILENO) == -1)
		return (perror("dup2 heredoc"), close(cmd->pipe_fd[0]), 1);
	close(cmd->pipe_fd[0]);
	return (0);
}

int	apply_redirections(t_command *cmd)
{
	if (redirect_input(cmd))
		return (1);
	if (redirect_output_trunc(cmd))
		return (1);
	if (redirect_output_append(cmd))
		return (1);
	if (redirect_heredoc(cmd))
		return (1);
	return (0);
}
