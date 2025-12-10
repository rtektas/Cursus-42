/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils-exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:23:45 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:26:43 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_cmd_not_found(t_command *cmd, t_minishell **ms)
{
	write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
	write(2, ": command not found\n", sizeof(": command not found\n") - 1);
	(*ms)->last_status = 127;
	return (127);
}

char	**ft_find_path(char **envp)
{
	char	**path;
	int		i;
	char	*temp;

	i = 0;
	path = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = ft_split(envp[i], ':');
			if (!path)
				return (NULL);
			break ;
		}
		i++;
	}
	if (path)
	{
		temp = path[0];
		path[0] = ft_substr(temp, 5, ft_strlen(temp));
		free(temp);
	}
	return (path);
}

void	setup_child_fds(int pipefd[2])
{
	if (pipefd[0] != -1)
		dup2(pipefd[0], STDIN_FILENO);
	if (pipefd[1] != -1)
		dup2(pipefd[1], STDOUT_FILENO);
	if (pipefd[0] != -1)
		close(pipefd[0]);
	if (pipefd[1] != -1)
		close(pipefd[1]);
}

/*char	*find_executable(t_command *cmd, char **paths)
{
	int		i;
	char	*candidate;

	if (cmd->argv && cmd->argv[0] && strchr(cmd->argv[0], '/'))
		return (cmd->argv[0]);
	i = 0;
	while (paths && paths[i])
	{
		candidate = ft_strjoin(paths[i], cmd->argv[0]);
		if (!candidate)
			return (NULL);
		if (access(candidate, X_OK) == 0)
			return (candidate);
		free(candidate);
		i++;
	}
	return (NULL);
}*/
