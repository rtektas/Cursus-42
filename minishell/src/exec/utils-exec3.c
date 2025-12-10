/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils-exec3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:00:00 by marvin            #+#    #+#             */
/*   Updated: 2025/11/07 17:00:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <errno.h>

int	path_status(const char *path)
{
	struct stat	st;

	if (access(path, F_OK) != 0)
		return (ENOENT);
	if (stat(path, &st) != 0)
		return (ENOENT);
	if (S_ISDIR(st.st_mode))
		return (EISDIR);
	if (access(path, X_OK) != 0)
		return (EACCES);
	return (0);
}

int	is_directory_path(const char *path)
{
	struct stat	st;

	if (!path)
		return (0);
	if (stat(path, &st) != 0)
		return (0);
	return (S_ISDIR(st.st_mode));
}

char	*dup_direct_path(char *arg)
{
	char	*candidate;

	candidate = ft_strdup(arg);
	if (!candidate)
		errno = ENOMEM;
	return (candidate);
}

static void	update_last_errno(int status, int *last_errno)
{
	if (status == EACCES)
		*last_errno = EACCES;
}

char	*search_in_paths(t_command *cmd, char **paths)
{
	int		i;
	char	*candidate;
	int		status;
	int		last_errno;

	i = 0;
	last_errno = ENOENT;
	while (paths[i])
	{
		candidate = ft_strjoin(paths[i], cmd->argv[0]);
		if (!candidate)
			return (errno = ENOMEM, NULL);
		status = path_status(candidate);
		if (status == 0)
			return (candidate);
		update_last_errno(status, &last_errno);
		free(candidate);
		i++;
	}
	errno = last_errno;
	return (NULL);
}
