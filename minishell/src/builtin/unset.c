/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 20:02:30 by asaracut          #+#    #+#             */
/*   Updated: 2025/11/05 16:29:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	remove_env(char *key, char ***env)
{
	int		i;
	int		j;
	size_t	key_len;
	int		removed;

	key_len = ft_strlen(key);
	i = 0;
	j = 0;
	removed = 0;
	while ((*env)[i])
	{
		if (!removed && ft_strncmp((*env)[i], key, key_len) == 0
			&& (*env)[i][key_len] == '=')
		{
			free((*env)[i]);
			removed = 1;
		}
		else
			(*env)[j++] = (*env)[i];
		i++;
	}
	(*env)[j] = NULL;
	return (0);
}

int	ft_unset(char **args, char ***env)
{
	int	i;

	i = 1;
	if (!args[1])
		return (0);
	while (args[i])
	{
		if (remove_env(args[i], env) != 0)
			ft_printf("unset: `%s': not a valid identifier\n", args[i]);
		i++;
	}
	return (0);
}
