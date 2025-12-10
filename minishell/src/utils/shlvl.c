/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 20:29:08 by marvin            #+#    #+#             */
/*   Updated: 2025/11/07 20:29:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	next_shlvl_value(const char *current)
{
	int	lvl;

	if (!current || !*current)
		return (1);
	lvl = ft_atoi(current) + 1;
	if (lvl <= 0)
		lvl = 1;
	if (lvl > 999)
		lvl = 1;
	return (lvl);
}

void	bump_shlvl(t_minishell *minishell)
{
	char	*current;
	char	*lvl_str;
	int		lvl;

	if (!minishell || !minishell->envp)
		return ;
	current = get_env_value("SHLVL", minishell->envp);
	lvl = next_shlvl_value(current);
	lvl_str = ft_itoa(lvl);
	if (!lvl_str)
		return ;
	if (add_or_update_env("SHLVL", lvl_str, &minishell->envp) != 0)
		perror("minishell");
	free(lvl_str);
}
