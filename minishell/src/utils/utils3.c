/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:07:29 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:26:43 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	print_sorted_env_line(char *line)
{
	int		j;
	char	*eq;

	eq = ft_strchr(line, '=');
	ft_printf("declare -x ");
	if (eq)
	{
		j = 0;
		while (line[j] && line[j] != '=')
		{
			ft_printf("%c", line[j]);
			j++;
		}
		ft_printf("=\"%s\"\n", eq + 1);
	}
	else
		ft_printf("%s\n", line);
}

int	build_new_value(char *old, char *val, char **out)
{
	if (old)
		*out = ft_strjoin(old, val);
	else
		*out = ft_strdup(val);
	if (!*out)
	{
		perror("export");
		return (1);
	}
	return (0);
}

void	export_print_sorted_env(char **env)
{
	char	**sorted;
	int		count;
	int		i;

	count = 0;
	while (env[count])
		count++;
	sorted = malloc(sizeof(char *) * (count + 1));
	if (!sorted)
		return ;
	i = 0;
	while (i < count)
	{
		sorted[i] = env[i];
		i++;
	}
	sorted[count] = NULL;
	qsort(sorted, count, sizeof(char *), cmp_env);
	i = 0;
	while (i < count)
	{
		print_sorted_env_line(sorted[i]);
		i++;
	}
	free(sorted);
}
