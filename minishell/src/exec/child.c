/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:45:00 by marvin            #+#    #+#             */
/*   Updated: 2025/11/05 16:45:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <errno.h>

/* moved to single.c: fork_child_with_bin */

static void	set_child_context(int pipefd[2])
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_child_fds(pipefd);
}

static void	handle_no_bin_error(t_command *cmd)
{
	if (errno == ENOMEM)
	{
		perror("malloc");
		exit(1);
	}
	if (errno == EACCES)
	{
		ms_perror(cmd->argv[0], "Permission denied");
		exit(126);
	}
	if (errno == EISDIR)
	{
		ms_perror(cmd->argv[0], "is a directory");
		exit(126);
	}
	ms_perror(cmd->argv[0], "command not found");
	exit(127);
}

static void	exec_child_work(t_command *cmd, char **paths, t_minishell **ms)
{
	char	*bin;
	int		st;

	if (apply_redirections(cmd))
		exit(1);
	if (cmd->argv && ft_is_built(cmd->argv) == 0)
	{
		st = exec_builtin(cmd->argv, &(*ms)->envp);
		exit(st);
	}
	errno = 0;
	bin = find_executable(cmd, paths);
	if (!bin)
		handle_no_bin_error(cmd);
	exec_command(cmd, bin, *ms);
}

static void	close_pipe_ends(int pipefd[2])
{
	if (pipefd[0] != -1)
		close(pipefd[0]);
	if (pipefd[1] != -1)
		close(pipefd[1]);
}

void	fork_child(t_command *cmd, char **paths, t_minishell **ms,
		int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		(*ms)->last_status = 1;
		return ;
	}
	if (pid == 0)
	{
		set_child_context(pipefd);
		exec_child_work(cmd, paths, ms);
	}
	close_pipe_ends(pipefd);
}
