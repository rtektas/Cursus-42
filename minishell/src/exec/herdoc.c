/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 03:13:25 by asaracut          #+#    #+#             */
/*   Updated: 2025/11/11 15:23:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	read_heredoc_lines(t_command *cmd)
{
	char	*line;
	size_t	len;

	len = ft_strlen(cmd->heredoc_delim);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, cmd->heredoc_delim, len) == 0 && line[len] == '\0')
		{
			free(line);
			break ;
		}
		write(cmd->pipe_fd[1], line, ft_strlen(line));
		write(cmd->pipe_fd[1], "\n", 1);
		free(line);
	}
}

static void	child_heredoc(t_command *cmd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(cmd->pipe_fd[0]);
	read_heredoc_lines(cmd);
	close(cmd->pipe_fd[1]);
	exit(0);
}

static int	parent_wait_heredoc(pid_t pid, t_command *cmd, t_minishell **ms)
{
	int	status;

	close(cmd->pipe_fd[1]);
	cmd->pipe_fd[1] = -1;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	if (WIFSIGNALED(status))
	{
		if (ms && *ms)
			(*ms)->last_status = 130;
		return (1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return (1);
	return (0);
}

static int	handle_heredoc(t_command *cmd, t_minishell **ms)
{
	pid_t	pid;

	if (pipe(cmd->pipe_fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(cmd->pipe_fd[0]);
		close(cmd->pipe_fd[1]);
		return (1);
	}
	if (pid == 0)
	{
		child_heredoc(cmd);
	}
	return (parent_wait_heredoc(pid, cmd, ms));
}

int	prepare_heredocs(t_command *cmd, t_minishell **ms)
{
	while (cmd)
	{
		if (cmd->heredoc && cmd->heredoc_delim)
		{
			if (handle_heredoc(cmd, ms) != 0)
				return (1);
		}
		cmd = cmd->next;
	}
	return (0);
}
