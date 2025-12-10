/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils-exec2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:23:45 by asaracut          #+#    #+#             */
/*   Updated: 2025/11/11 16:14:34 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <errno.h>

void	ms_perror(const char *cmd, const char *msg)
{
	if (!cmd || !msg)
		return ;
	write(STDERR_FILENO, cmd, ft_strlen(cmd));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
}

void	exec_command(t_command *cmd, char *bin, t_minishell *ms)
{
	if (!bin)
	{
		ms_perror(cmd->argv[0], "command not found");
		exit(127);
	}
	if (is_directory_path(bin))
	{
		ms_perror(cmd->argv[0], "is a directory");
		exit(126);
	}
	execve(bin, cmd->argv, ms->envp);
	perror(cmd->argv[0]);
	if (errno == ENOENT)
		exit(127);
	else if (errno == EACCES)
		exit(126);
	else
		exit(126);
}

char	*find_executable(t_command *cmd, char **paths)
{
	errno = 0;
	if (cmd->argv[0] && ft_strchr(cmd->argv[0], '/'))
		return (dup_direct_path(cmd->argv[0]));
	if (!paths)
		return (errno = ENOENT, NULL);
	return (search_in_paths(cmd, paths));
}
