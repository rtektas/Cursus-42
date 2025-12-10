/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:07:29 by asaracut          #+#    #+#             */
/*   Updated: 2025/11/04 12:00:35 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	fill_append_key(char *arg, char *key, char *plus_eq)
{
	size_t	len;

	len = plus_eq - arg;
	if (len >= 1024)
		return (0);
	ft_strlcpy(key, arg, len + 1);
	if (!is_valid_identifier(key))
	{
		ft_printf("export: `%s': not a valid identifier\n", arg);
		return (0);
	}
	return (1);
}

static int	handle_export_append(char *arg, char ***env)
{
	char	key[1024];
	char	*plus_eq;
	char	*old;
	char	*val;
	char	*new_val;

	plus_eq = ft_strnstr(arg, "+=", ft_strlen(arg));
	if (!plus_eq)
		return (1);
	if (!fill_append_key(arg, key, plus_eq))
		return (1);
	val = plus_eq + 2;
	old = get_env_value(key, *env);
	if (build_new_value(old, val, &new_val))
		return (1);
	if (add_or_update_env(key, new_val, env) != 0)
	{
		free(new_val);
		perror("export");
		return (1);
	}
	free(new_val);
	return (0);
}

static int	handle_export_assignment(char *arg, char ***env)
{
	char	key[1024];
	char	*equal_sign;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	ft_strlcpy(key, arg, equal_sign - arg + 1);
	if (!is_valid_identifier(key))
		return (ft_printf("export: `%s': not a valid identifier\n", arg), 1);
	value = equal_sign + 1;
	if (add_or_update_env(key, value, env))
		return (perror("export"), 1);
	return (0);
}

static int	handle_export_no_value(char *arg, char ***env)
{
	char	key[1024];

	ft_strlcpy(key, arg, sizeof(key));
	if (!is_valid_identifier(key))
		return (ft_printf("export: `%s': not a valid identifier\n", arg), 1);
	if (add_or_update_env(key, "", env))
		return (perror("export"), 1);
	return (0);
}

int	ft_export(char **args, char ***env)
{
	int	i;

	if (!args[1])
		return (export_print_sorted_env(*env), 0);
	i = 1;
	while (args[i])
	{
		if (ft_strnstr(args[i], "+=", ft_strlen(args[i])))
		{
			if (handle_export_append(args[i], env))
				return (1);
		}
		else if (ft_strchr(args[i], '='))
		{
			if (handle_export_assignment(args[i], env))
				return (1);
		}
		else
		{
			if (handle_export_no_value(args[i], env))
				return (1);
		}
		i++;
	}
	return (0);
}
