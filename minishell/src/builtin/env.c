/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:07:29 by asaracut          #+#    #+#             */
/*   Updated: 2025/11/11 13:32:15 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	append_env(char ***env, char *entry)
{
	int		count;
	char	**new_env;
	int		i;

	count = 0;
	while ((*env)[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
	{
		free(entry);
		return (-1);
	}
	i = 0;
	while (i < count)
	{
		new_env[i] = (*env)[i];
		i++;
	}
	new_env[count] = entry;
	new_env[count + 1] = NULL;
	free(*env);
	*env = new_env;
	return (0);
}

int	env_key_index(char **env, const char *key)
{
	int		index;
	size_t	len;

	if (!env || !key)
		return (-1);
	len = ft_strlen(key);
	index = 0;
	while (env[index])
	{
		if (ft_strncmp(env[index], key, len) == 0 && (env[index][len] == '='
			|| env[index][len] == '\0'))
			return (index);
		index++;
	}
	return (-1);
}

char	*build_env_entry(char *key, char *value)
{
	char	*entry;
	size_t	len;

	len = ft_strlen(key) + ft_strlen(value) + 2;
	entry = malloc(len);
	if (!entry)
		return (NULL);
	ft_strlcpy(entry, key, len);
	ft_strlcat(entry, "=", len);
	ft_strlcat(entry, value, len);
	return (entry);
}

int	add_or_update_env(char *key, char *value, char ***env)
{
	char	*entry;
	int		idx;

	entry = build_env_entry(key, value);
	if (!entry)
		return (-1);
	idx = env_key_index(*env, key);
	if (idx >= 0)
	{
		free((*env)[idx]);
		(*env)[idx] = entry;
		return (0);
	}
	return (append_env(env, entry));
}
