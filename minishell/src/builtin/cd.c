/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:02:58 by asaracut          #+#    #+#             */
/*   Updated: 2025/11/11 14:38:57 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <errno.h>
#include <string.h>

static char	*get_cd_target(char **args, char **env)
{
	char	*path;

	path = args[1];
	if (!path)
	{
		path = (char *)get_env_value("HOME", env);
		if (!path)
		{
			ft_printf("minishell: cd: HOME not set\n");
			return (NULL);
		}
	}
	else if (ft_strcmp(path, "-") == 0)
	{
		path = (char *)get_env_value("OLDPWD", env);
		if (!path)
		{
			ft_printf("minishell: cd: OLDPWD not set\n");
			return (NULL);
		}
		ft_printf("%s\n", path);
	}
	return (path);
}

static int	cd_check_args(char **args)
{
	if (args[1] && args[2])
	{
		ft_printf("minishell: cd: too many arguments\n");
		return (1);
	}
	return (0);
}

static int	cd_set_oldpwd(char oldpwd[100])
{
	if (getcwd(oldpwd, 100) == NULL)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

static int	cd_chdir_and_err(const char *path)
{
	if (chdir(path) != 0)
	{
		ft_printf("minishell: cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	return (0);
}

int	ft_cd(char **args, char ***env)
{
	char	*path;
	char	cwd[100];
	char	oldpwd[100];

	if (cd_check_args(args))
		return (1);
	if (cd_set_oldpwd(oldpwd))
		return (1);
	path = get_cd_target(args, *env);
	if (!path)
		return (1);
	if (cd_chdir_and_err(path))
		return (1);
	add_or_update_env("OLDPWD", oldpwd, env);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		add_or_update_env("PWD", cwd, env);
	return (0);
}
