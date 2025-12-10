/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:37:51 by rtektas           #+#    #+#             */
/*   Updated: 2025/06/11 17:37:51 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	exec_builtin(char **args, char ***env)
{
	if (!args || !args[0])
		return (1);
	if (ft_strcmp(args[0], "echo") == 0)
		return (ft_echo(args));
	if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(args, env));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_pwd(args));
	if (ft_strcmp(args[0], "export") == 0)
		return (ft_export(args, env));
	if (ft_strcmp(args[0], "unset") == 0)
		return (ft_unset(args, env));
	if (ft_strcmp(args[0], "env") == 0)
		return (ft_env(args, *env));
	if (ft_strcmp(args[0], "exit") == 0)
		return (ft_exit(args));
	return (1);
}

int	ft_env(char **args, char **env)
{
	int	i;

	i = 0;
	if (args[1])
	{
		ft_printf("env: '%s': No such file or directory\n", args[1]);
		return (127);
	}
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
			ft_printf("%s\n", env[i]);
		i++;
	}
	return (0);
}
