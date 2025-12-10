/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:43:26 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:26:43 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_strcmp(char const *s1, char const *s2)
{
	int	i;

	i = 0;
	while ((s1[i] == s2[i]) && s1[i] && s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	count_cmds(const t_command *cmd)
{
	const t_command	*cur;
	int				count;

	count = 0;
	cur = cmd;
	while (cur)
	{
		++count;
		cur = cur->next;
	}
	return (count);
}

void	ft_add_slash(char **paths)
{
	int		i;
	char	*old;

	i = 0;
	while (paths[i])
	{
		old = paths[i];
		paths[i] = ft_strjoin(old, "/");
		free(old);
		i++;
	}
}

int	ft_is_built(char **arg)
{
	if (ft_strcmp(arg[0], "echo") == 0)
		return (0);
	else if (ft_strcmp(arg[0], "cd") == 0)
		return (0);
	else if (ft_strcmp(arg[0], "pwd") == 0)
		return (0);
	else if (ft_strcmp(arg[0], "export") == 0)
		return (0);
	else if (ft_strcmp(arg[0], "unset") == 0)
		return (0);
	else if (ft_strcmp(arg[0], "env") == 0)
		return (0);
	else if (ft_strcmp(arg[0], "exit") == 0)
		return (0);
	return (1);
}
