/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:45:10 by marvin            #+#    #+#             */
/*   Updated: 2025/11/05 16:45:10 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <errno.h>

void	fork_child_with_bin(t_command *cmd, t_minishell **ms, char *bin)
{
	pid_t	pid;
	int		fd_dummy[2];

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		(*ms)->last_status = 1;
		return ;
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		fd_dummy[0] = -1;
		fd_dummy[1] = -1;
		setup_child_fds(fd_dummy);
		if (apply_redirections(cmd))
			exit(1);
		exec_command(cmd, bin, *ms);
	}
}

static int	handle_find_exec_error(t_command *cmd, t_minishell **ms)
{
	if (errno == ENOMEM)
	{
		perror("malloc");
		(*ms)->last_status = 1;
		return ((*ms)->last_status);
	}
	if (errno == EACCES)
	{
		ms_perror(cmd->argv[0], "Permission denied");
		(*ms)->last_status = 126;
		return ((*ms)->last_status);
	}
	if (errno == EISDIR)
	{
		ms_perror(cmd->argv[0], "is a directory");
		(*ms)->last_status = 126;
		return ((*ms)->last_status);
	}
	return (handle_cmd_not_found(cmd, ms));
}

static int	launch_external_single(t_command *cmd, t_minishell **ms,
		char **paths)
{
	int		status;
	char	*bin;

	errno = 0;
	bin = find_executable(cmd, paths);
	if (!bin)
		return (handle_find_exec_error(cmd, ms));
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	fork_child_with_bin(cmd, ms, bin);
	free(bin);
	wait(&status);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	if (WIFEXITED(status))
		(*ms)->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		(*ms)->last_status = 128 + WTERMSIG(status);
	return ((*ms)->last_status);
}

int	exec_single(t_command *cmd, t_minishell **ms, char **paths)
{
	if (!cmd || !cmd->argv)
		return (1);
	if (!cmd->argv[0] || cmd->argv[0][0] == '\0')
		return (exec_only_redirections(cmd, ms));
	if (ft_is_built(cmd->argv) == 0)
	{
		(*ms)->last_status = run_builtin_with_redir(cmd, ms);
		return ((*ms)->last_status);
	}
	return (launch_external_single(cmd, ms, paths));
}
